/*
This software is subject to the license described in the License.txt file 
included with this software distribution. You may not use this file except in compliance 
with this license.

Copyright (c) Dynastream Innovations Inc. 2012
All rights reserved.
*/
 

#include "StdAfx.h"
#include "RacquetDisplay.h"

void RacquetDisplay::InitializeSim()
{
}

void RacquetDisplay::ANT_eventNotification(UCHAR ucEventCode_, UCHAR *pucEventBuffer_)
{
	UCHAR ucPageNum = pucEventBuffer_[0];

	switch(ucEventCode_)
	{
	case EVENT_RX_ACKNOWLEDGED:
	case EVENT_RX_BURST_PACKET:
	case EVENT_RX_BROADCAST:
		// Check if incoming page is a common page or a racquet page
		if(ucPageNum < Common->START_COMMON_PAGE)
			RacquetData->Decode(pucEventBuffer_);
		else
			Common->Decode(pucEventBuffer_);

		// Update the UI 
		UpdateDisplay(ucPageNum);
		break;

		case EVENT_TRANSFER_TX_COMPLETED:
			ucAckRetryCount = 0;			// Reset retransmission counter
			ucMsgExpectingAck = 0;			// Clear pending msg code
			UpdateDisplayAckStatus(RacquetData->ACK_SUCCESS);	// Tx successful
			break;
		case EVENT_TRANSFER_TX_FAILED:	
		case EVENT_ACK_TIMEOUT:		// Intentional fall thru
			if(ucMsgExpectingAck)
			{
				if(HandleRetransmit())
					UpdateDisplayAckStatus(RacquetData->ACK_RETRY);	// Data was retransmitted
				else
					UpdateDisplayAckStatus(RacquetData->ACK_FAIL);	// Maximum number of retries exceeded, Tx failed
			}
			break;
	
	case EVENT_RX_FAIL:
		break;

	default:
		break;
	}
}
void RacquetDisplay::UpdateDisplay(UCHAR ucPageNum_)
{
	switch(ucPageNum_)
	{
	case (UCHAR)AntPlusRacquet::PAGE_STROKE_EVENT:
		lbEventCount->Text = RacquetData->ucEventCount.ToString();
		lbEventTime->Text = RacquetData->usEventTime.ToString();
		lbStrokeType->Text = GetStrokeText(RacquetData->ucEventType, ucPageNum_);
		lbRacquetZone->Text = GetZoneText(RacquetData->ucRacquetZone, ucPageNum_);
		lbANTFS->Text = GetANTFSText (RacquetData->ucANTFS, ucPageNum_);
		lbBallSpeed->Text = (RacquetData->usSpeed == AntPlusRacquet::UNKNOWN ?
							"Invalid" :
                     rbMPS->Checked ?
                     ((double)RacquetData->usSpeed / 100).ToString("F2") : 
                     Math::Round((((double)RacquetData->usSpeed / 100) * ((double)36 / (double)10)), 3, System::MidpointRounding::AwayFromZero).ToString("F2"));
		
		lbDisplayZone->Text = GetZoneText(RacquetData->ucRacquetZone, ucPageNum_);
	//	lbDisplayANTFS->Text = GetANTFSText (RacquetData->ucANTFS, ucPageNum_);
		lbDisplayEvent->Text = RacquetData->ucEventCount.ToString();
		lbDisplayStroke->Text = GetStrokeText(RacquetData->ucEventType, ucPageNum_);
		lbDisplayTime->Text = RacquetData->usEventTime.ToString();
		lbDisplaySpeed->Text = (RacquetData->usSpeed == AntPlusRacquet::UNKNOWN ?
							"Invalid" :
                     rbMPS->Checked ?
                     ((double)RacquetData->usSpeed / 100).ToString("F2") :
                     Math::Round((((double)RacquetData->usSpeed / 100) * ((double)36 / (double)10)), 3, System::MidpointRounding::AwayFromZero).ToString("F2"));
		break;

	case (UCHAR)AntPlusRacquet::PAGE_STROKE_COUNT:
		lbStrokeType1->Text = GetStrokeText(RacquetData->ucStrokeType1, ucPageNum_);
		lbStrokeType2->Text = GetStrokeText(RacquetData->ucStrokeType2, ucPageNum_);
		lbStrokeCount1->Text = (RacquetData->usStrokeCount1 == AntPlusRacquet::UNKNOWN ?
								"Unknown" : 
								RacquetData->usStrokeCount1.ToString());
		lbStrokeCount2->Text = (RacquetData->usStrokeCount2 == AntPlusRacquet::UNKNOWN ?
								"Unknown" :
								RacquetData->usStrokeCount2.ToString());
		break;

	case (UCHAR)AntPlusRacquet::PAGE_PREVIOUS_STROKE_EVENT:
		lbPreviousCount->Text = RacquetData->ucPreviousEventCount.ToString();
		lbPreviousType->Text = GetStrokeText(RacquetData->ucPreviousEventType, ucPageNum_);
		lbPreviousTime->Text = RacquetData->usPreviousEventTime.ToString();
		lbPreviousZone->Text = GetZoneText(RacquetData->ucPreviousRacquetZone, ucPageNum_);
		lbPreviousSpeed->Text = (RacquetData->usPreviousSpeed == AntPlusRacquet::UNKNOWN ?
							"Invalid" :
							((double)RacquetData->usPreviousSpeed / 100).ToString());
		break;

	case (UCHAR)AntPlusRacquet::PAGE_ZONE_COUNT:
		lbZoneStroke->Text = GetStrokeText(RacquetData->ucType, ucPageNum_);
		lbZone1->Text = GetZoneText(RacquetData->ucZone, ucPageNum_);
		if ((UCHAR)(RacquetData->ucZone + 1) == 0)
		{
			lbZone2->Text = "N/A";
		}

		else if(RacquetData->ucZone + 1 == (UCHAR) AntPlusRacquet::RacquetZones::UNKNOWN)
		{
			lbZone2->Text = "Unknown";
		}

		else 
		{
			lbZone2->Text = GetZoneText (RacquetData->ucZone++ , ucPageNum_);
		}
		
	
		lbZoneCount1->Text = (RacquetData->usCount == AntPlusRacquet::UNKNOWN ?
						"Unknown" :
						RacquetData->usCount.ToString());
		lbZoneCount2->Text = (RacquetData->usCountPlus1 == AntPlusRacquet::UNKNOWN ?
						"Unknown" :
						RacquetData->usCountPlus1.ToString());
		break;

	case (UCHAR)CommonData::PAGE80:
		lbHardwareVer->Text = Common->ucHwVersion.ToString();
		lbManfId->Text = Common->usMfgID.ToString();
		lbModelNumber->Text = Common->usModelNum.ToString();
		break;

	case (UCHAR)CommonData::PAGE81:
		lbSoftwareVer->Text = Common->ucSwVersion.ToString();
		lbSerialNumber->Text = (Common->ulSerialNum == 0xFFFFFFFF ?
							"Invalid" :
							Common->ulSerialNum.ToString());
		break;
      
   case (UCHAR)CommonData::PAGE82:
      if(Common->usBatVoltage256 == CommonData::BATTERY_VOLTAGE_INVALID)
         labelBattVolt->Text = "Invalid";
      else
         labelBattVolt->Text = System::Math::Round((double)Common->usBatVoltage256/256, 4).ToString("N2");

      if(Common->eTimeResolution == CommonData::TimeResolution::SIXTEEN)
         labelTimeRes->Text = "16 s";
      else
         labelTimeRes->Text = "2 s";

      labelOpTime->Text = (Common->ulOpTime * (UCHAR)Common->eTimeResolution).ToString();

      switch(Common->eBatStatus)
      {
      case CommonData::BatStatus::CRITICAL:
         labelBattStatus->Text = "Critical";
         break;
      case CommonData::BatStatus::GOOD:
         labelBattStatus->Text = "Good";
         break;
      case CommonData::BatStatus::INVALID:
         labelBattStatus->Text = "Invalid";
         break;
      case CommonData::BatStatus::LOW:
         labelBattStatus->Text = "Low";
         break;
      case CommonData::BatStatus::NEW:
         labelBattStatus->Text = "New";
         break;
      case CommonData::BatStatus::OK:
         labelBattStatus->Text = "Ok";
         break;
      default:
         labelBattStatus->Text = "---";
         break;
      }
      break;

	default:
		break;
	}
}

System::String^ RacquetDisplay::GetStrokeText(UCHAR ucRacquetData_, UCHAR ucPageNum_)
{
	System::String^ strokeText;

	switch(ucRacquetData_)
	{
	case (UCHAR)AntPlusRacquet::StrokeTypes::NO_EVENT:
		switch(ucPageNum_)
		{
		case (UCHAR)AntPlusRacquet::PAGE_PREVIOUS_STROKE_EVENT:
		case (UCHAR)AntPlusRacquet::PAGE_STROKE_EVENT:
			strokeText = "No Event";
			break;

		default:
			strokeText = "All Events";
			break;
		}
		break;

	case (UCHAR)AntPlusRacquet::StrokeTypes::OTHER:
		strokeText = "Other";
		break;

	case (UCHAR)AntPlusRacquet::StrokeTypes::SERVE:
		strokeText = "Serve";
		break;

	case (UCHAR)AntPlusRacquet::StrokeTypes::FOREHAND:
		strokeText = "Forehand";
		break;

	case (UCHAR)AntPlusRacquet::StrokeTypes::BACKHAND:
		strokeText = "Backhand";
		break;

	case (UCHAR)AntPlusRacquet::StrokeTypes::SMASH:
		strokeText = "Smash";
		break;

	case (UCHAR)AntPlusRacquet::StrokeTypes::UNKNOWN:
		strokeText = "Unknown";
		break;

	default:
		strokeText = "----";
		break;
	}

	return strokeText;
}
System::String^ RacquetDisplay::GetZoneText(UCHAR ucRacquetData_, UCHAR ucPageNum_)
{
	System::String^ zoneText;

	switch(ucRacquetData_)
	{
	case(UCHAR)AntPlusRacquet::RacquetZones::MISS:
		zoneText = "Miss";
		break;

	case(UCHAR)AntPlusRacquet::RacquetZones::TOP_RIGHT:
		zoneText = "Top Right";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::MIDDLE_RIGHT:
		zoneText = "Middle Right";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::BOTTOM_RIGHT:
		zoneText = "Bottom Right";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::BOTTOM_CENTRE:
		zoneText = "Bottom Centre";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::BOTTOM_LEFT:
		zoneText = "Bottom Left";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::MIDDLE_CENTRE:
		zoneText = "Middle Centre";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::MIDDLE_LEFT:
		zoneText = "Middle Left";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::TOP_LEFT:
		zoneText = "Top Left";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::TOP_CENTRE:
		zoneText = "Top Centre";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::RIM_OTHER:
		zoneText = "Rim/Other";
		break;

	case (UCHAR)AntPlusRacquet::RacquetZones::UNKNOWN:
		zoneText = "Unknown";
		break;

	default:
		zoneText = "----";
		break;
	}

	return zoneText;
}
System::String^ RacquetDisplay::GetANTFSText(UCHAR ucRacquetData_, UCHAR ucPageNum_)
{
	System::String^ aNTFSText;
	switch(ucRacquetData_)
	{
	case(UCHAR)AntPlusRacquet::ANTFS::SUPPORTED:
		aNTFSText = "Supported";
		break;

	case(UCHAR)AntPlusRacquet::ANTFS::NOT_SUPPORTED:
		aNTFSText = "Not Supported";
		break;

	default:
		aNTFSText = "---";
		break;
	}

	return aNTFSText;
}




void RacquetDisplay::onTimerTock(USHORT eventTime)
{
}
void RacquetDisplay::btRequestEvent_Click(System::Object ^sender, System::EventArgs ^e)
{
	UCHAR aucAckBuffer[8] = {0,0,0,0,0,0,0,0};

	Common->ucDescriptorByte1 = (UCHAR)nmPreviousEvent->Value; 
	Common->ucDescriptorByte2 = (UCHAR)AntPlusRacquet::UNKNOWN;
	Common->ucReqTransResp = (UCHAR)nmTxResponsePrevious->Value;

	Common->ucReqPageNum = AntPlusRacquet::PAGE_PREVIOUS_STROKE_EVENT;
	Common->ucCommandType = CommonData::COMMAND_TYPE_DATA;

	SendAckMessage(Common->PAGE70);
}
void RacquetDisplay::btRequestStrokeCount_Click(System::Object ^sender, System::EventArgs ^e)
{
	UCHAR aucAckBuffer[8] = {0,0,0,0,0,0,0,0};

	Common->ucDescriptorByte1 = (cbStrokeType1->SelectedIndex == AntPlusRacquet::NUMBER_OF_STROKE_TYPES - 1 ? // Unknown 
								(UCHAR)AntPlusRacquet::StrokeTypes::UNKNOWN : 
								cbStrokeType1->SelectedIndex);
	Common->ucDescriptorByte2 = (cbStrokeType2->SelectedIndex == AntPlusRacquet::NUMBER_OF_STROKE_TYPES - 1 ? // Unknown
								(UCHAR)AntPlusRacquet::StrokeTypes::UNKNOWN :
								cbStrokeType2->SelectedIndex);
	Common->ucReqTransResp = (UCHAR)nmTxResponseStrokes->Value;
	
	Common->ucReqPageNum = AntPlusRacquet::PAGE_STROKE_COUNT;
	Common->ucCommandType = CommonData::COMMAND_TYPE_DATA;

	Common->Encode(CommonData::PAGE70, aucAckBuffer);

	SendAckMessage(Common->PAGE70);
}
void RacquetDisplay::btRequestZoneCount_Click(System::Object ^sender, System::EventArgs ^e)
{
	UCHAR aucAckBuffer[8] = {0,0,0,0,0,0,0,0};

	Common->ucDescriptorByte1 = (cbStrokeType->SelectedIndex == AntPlusRacquet::NUMBER_OF_STROKE_TYPES - 1 ? // Unknown
								(UCHAR)AntPlusRacquet::StrokeTypes::UNKNOWN :
								cbStrokeType->SelectedIndex);
	Common->ucDescriptorByte2 = (cbZoneType->SelectedIndex == AntPlusRacquet::NUMBER_OF_ZONES - 1 ? // Unknown
								(UCHAR)AntPlusRacquet::RacquetZones::UNKNOWN :
								cbZoneType->SelectedIndex);
	Common->ucReqTransResp = (UCHAR)nmTxResponseZones->Value;

	Common->ucReqPageNum = AntPlusRacquet::PAGE_ZONE_COUNT;
	Common->ucCommandType = CommonData::COMMAND_TYPE_DATA;

	Common->Encode(CommonData::PAGE70, aucAckBuffer);

	SendAckMessage(Common->PAGE70);
}
void RacquetDisplay::UpdateDisplayAckStatus(UCHAR ucStatus_)
{
	switch(ucStatus_)
	{
		case RacquetData->ACK_SUCCESS:
			lbAckStatus->ForeColor = System::Drawing::Color::Green;
			lbAckStatus->Text = "SENT";
			break;
		case RacquetData->ACK_RETRY:
			lbAckStatus->ForeColor = System::Drawing::Color::Blue;
			lbAckStatus->Text = "RTRY";
			break;
		case RacquetData->ACK_FAIL:
			lbAckStatus->ForeColor = System::Drawing::Color::Red;
			lbAckStatus->Text = "FAIL";
			break;
		default:
			break;
	}
}
BOOL RacquetDisplay::HandleRetransmit()
{
	BOOL bSuccess = TRUE;

	if(ucMsgExpectingAck)	// Message still expecting an ack
	{
		if(ucAckRetryCount++ < RacquetData->MAX_RETRIES)
			SendAckMessage(ucMsgExpectingAck);
		else
			bSuccess = FALSE;
	}
	
	return bSuccess;
}
void RacquetDisplay::btSessionStart_Click(System::Object ^sender, System::EventArgs ^e)
{
	RacquetData->eEvent = AntPlusRacquet::FITEvents::SESSION;
	RacquetData->eEventType = AntPlusRacquet::FITEventTypes::START;
	RacquetData->ulEventData = AntPlusRacquet::UNKNOWN_ULONG;

	SendAckMessage(AntPlusRacquet::PAGE_EVENT_DATA);
}
void RacquetDisplay::btSessionStop_Click(System::Object ^sender, System::EventArgs ^e)
{
	RacquetData->eEvent = AntPlusRacquet::FITEvents::SESSION;
	RacquetData->eEventType = AntPlusRacquet::FITEventTypes::STOP;
	RacquetData->ulEventData = AntPlusRacquet::UNKNOWN_ULONG;

	SendAckMessage(AntPlusRacquet::PAGE_EVENT_DATA);
}
void RacquetDisplay::btTimerStart_Click(System::Object ^sender, System::EventArgs ^e)
{
   RacquetData->eEvent = AntPlusRacquet::FITEvents::TIMER;
   RacquetData->eEventType = AntPlusRacquet::FITEventTypes::START;
   RacquetData->ulEventData = AntPlusRacquet::UNKNOWN_ULONG;

   SendAckMessage(AntPlusRacquet::PAGE_EVENT_DATA);
}
void RacquetDisplay::btTimerStop_Click(System::Object ^sender, System::EventArgs ^e)
{
   RacquetData->eEvent = AntPlusRacquet::FITEvents::TIMER;
   RacquetData->eEventType = AntPlusRacquet::FITEventTypes::STOP;
   RacquetData->ulEventData = AntPlusRacquet::UNKNOWN_ULONG;

   SendAckMessage(AntPlusRacquet::PAGE_EVENT_DATA);
}
void RacquetDisplay::btMarkStroke_Click(System::Object ^sender, System::EventArgs ^e)
{
	RacquetData->eEvent = AntPlusRacquet::FITEvents::USER_MARKER;
	RacquetData->eEventType = AntPlusRacquet::FITEventTypes::MARKER;
	RacquetData->ulEventData = AntPlusRacquet::UNKNOWN_ULONG;

	SendAckMessage(AntPlusRacquet::PAGE_EVENT_DATA);
}
void RacquetDisplay::btSendGameScore_Click(System::Object ^sender, System::EventArgs ^e)
{
	RacquetData->eEvent = AntPlusRacquet::FITEvents::SPORT_POINT;
	RacquetData->eEventType = AntPlusRacquet::FITEventTypes::MARKER;

	// Clear the data so we can populate it
	RacquetData->ulEventData = 0x00;

	if(cbInvalidMyGameScore->Checked)
		RacquetData->ulEventData |= (ULONG)AntPlusRacquet::UNKNOWN;
	else
		RacquetData->ulEventData |= System::Convert::ToInt16(nmMyGameScore->Value);

	if(cbInvalidOpponentGameScore->Checked)
		RacquetData->ulEventData |= (ULONG)((ULONG)AntPlusRacquet::UNKNOWN << 16);
	else
		RacquetData->ulEventData |= (ULONG)(System::Convert::ToInt16(nmOpponentGameScore->Value) << 16);

	SendAckMessage(AntPlusRacquet::PAGE_EVENT_DATA);
}
void RacquetDisplay::btSendSessionData_Click(System::Object ^sender, System::EventArgs ^e)
{
	switch(cbGameMode->SelectedIndex)
	{
	case 0: // Warm Up
		RacquetData->eGameMode = AntPlusRacquet::FITSubSports::WARM_UP;
		break;
	case 1: // Match
		RacquetData->eGameMode = AntPlusRacquet::FITSubSports::MATCH;
		break;
	case 2: // Exercise
		RacquetData->eGameMode = AntPlusRacquet::FITSubSports::EXERCISE;
		break;
	case 3: // Challenge
		RacquetData->eGameMode = AntPlusRacquet::FITSubSports::CHALLENGE;
		break;
	default:
		break;
	}

	if(cbInvalidMySetsWon->Checked)
		RacquetData->usMySessionsWon = AntPlusRacquet::UNKNOWN;
	else
		RacquetData->usMySessionsWon = System::Convert::ToUInt16(nmMySetsWon->Value);
	
	if(cbInvalidOpponentSetsWon->Checked)
		RacquetData->usOpponentSessionsWon = AntPlusRacquet::UNKNOWN;
	else
		RacquetData->usOpponentSessionsWon = System::Convert::ToUInt16(nmOpponentSetsWon->Value);

	SendAckMessage(RacquetData->PAGE_SESSION_DATA);
}
void RacquetDisplay::btSendSetCount_Click(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidMySetCount->Checked)
		RacquetData->usMySetCount = AntPlusRacquet::UNKNOWN;
	else
		RacquetData->usMySetCount = System::Convert::ToUInt16(nmMySetCount->Value);
	
	if(cbInvalidOpponentSetCount->Checked)
		RacquetData->usOpponentSetCount = AntPlusRacquet::UNKNOWN;
	else
		RacquetData->usOpponentSetCount = System::Convert::ToUInt16(nmOpponentSetCount->Value);

	SendAckMessage(RacquetData->PAGE_LAP_DATA);
}
void RacquetDisplay::btSendGameCount_Click(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidMyGameCount->Checked)
		RacquetData->usMyGameCount = AntPlusRacquet::UNKNOWN;
	else
		RacquetData->usMyGameCount = System::Convert::ToUInt16(nmMyGameCount->Value);
	
	if(cbInvalidOpponentGameCount->Checked)
		RacquetData->usOpponentGameCount = AntPlusRacquet::UNKNOWN;
	else
		RacquetData->usOpponentGameCount = System::Convert::ToUInt16(nmOpponentGameCount->Value);

	SendAckMessage(RacquetData->PAGE_LENGTH_DATA);
}
void RacquetDisplay::cbInvalidMyGameCount_CheckedChanged(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidMyGameCount->Checked)
		nmMyGameCount->Enabled = false;
	else
		nmMyGameCount->Enabled = true;
}
void RacquetDisplay::cbInvalidOpponentGameCount_CheckedChanged(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidOpponentGameCount->Checked)
		nmOpponentGameCount->Enabled = false;
	else
		nmOpponentGameCount->Enabled = true;
}
void RacquetDisplay::cbInvalidMySetCount_CheckedChanged(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidMySetCount->Checked)
		nmMySetCount->Enabled = false;
	else
		nmMySetCount->Enabled = true;

}
void RacquetDisplay::cbInvalidMySetsWon_CheckedChanged(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidMySetsWon->Checked)
		nmMySetsWon->Enabled = false;
	else
		nmMySetsWon->Enabled = true;
}
void RacquetDisplay::cbInvalidOpponentSetCount_CheckedChanged(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidOpponentSetCount->Checked)
		nmOpponentSetCount->Enabled = false;
	else
		nmOpponentSetCount->Enabled = true;

}
void RacquetDisplay::cbInvalidOpponentSetsWon_CheckedChanged(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidOpponentSetsWon->Checked)
		nmOpponentSetsWon->Enabled = false;
	else 
		nmOpponentSetsWon->Enabled = true;
}
void RacquetDisplay::cbInvalidMyGameScore_CheckedChanged(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidMyGameScore->Checked)
		nmMyGameScore->Enabled = false;
	else
		nmMyGameScore->Enabled = true;
}
void RacquetDisplay::cbInvalidOpponentGameScore_CheckedChanged(System::Object ^sender, System::EventArgs ^e)
{
	if(cbInvalidOpponentGameScore->Checked)
		nmOpponentGameScore->Enabled = false;
	else
		nmOpponentGameScore->Enabled = true;
}
void RacquetDisplay::SendAckMessage(UCHAR ucMsgCode_)
{
	UCHAR aucAckBuffer[8] = {0,0,0,0,0,0,0,0};	

	switch(ucMsgCode_)
	{
		case AntPlusRacquet::PAGE_EVENT_DATA:
			RacquetData->Encode(AntPlusRacquet::PAGE_EVENT_DATA, aucAckBuffer);
			break;
		case AntPlusRacquet::PAGE_SESSION_DATA:
			RacquetData->Encode(AntPlusRacquet::PAGE_SESSION_DATA, aucAckBuffer);
			break;
		case AntPlusRacquet::PAGE_LAP_DATA:
			RacquetData->Encode(AntPlusRacquet::PAGE_LAP_DATA, aucAckBuffer);
			break;
		case AntPlusRacquet::PAGE_LENGTH_DATA:
			RacquetData->Encode(AntPlusRacquet::PAGE_LENGTH_DATA, aucAckBuffer);
			break;
      case AntPlusRacquet::PAGE_METABOLIC_DATA:
			RacquetData->Encode(AntPlusRacquet::PAGE_METABOLIC_DATA, aucAckBuffer);
         break;
      case AntPlusRacquet::PAGE_SPEED_DATA:
			RacquetData->Encode(AntPlusRacquet::PAGE_SPEED_DATA, aucAckBuffer);
         break;
      case AntPlusRacquet::PAGE_DISTANCE_DATA:
			RacquetData->Encode(AntPlusRacquet::PAGE_DISTANCE_DATA, aucAckBuffer);
         break;
		case CommonData::PAGE70:		 
			Common->Encode(CommonData::PAGE70, aucAckBuffer);
			break;
		default:
			return;		// Do not transmit if unsupported message code
	}

	// Send acknowledged message
	if(!ucMsgExpectingAck)	// If this message is not a retransmission
	{
		ucAckRetryCount = 0;			// Reset retransmission counter
		ucMsgExpectingAck = ucMsgCode_;	// Set code to message for which an ACK wil be expected
	}

	requestAckMsg(aucAckBuffer);	// Send message
}
void RacquetDisplay::rbMPS_CheckedChanged(System::Object ^sender, System::EventArgs ^e)
{
   if(lbBallSpeed->Text == "----")
      return;

   if(RacquetData->usSpeed == AntPlusRacquet::UNKNOWN)
      lbBallSpeed->Text = "Invalid";
   else
   {
      if(rbMPS->Checked)
         lbBallSpeed->Text = ((double)((double)RacquetData->usSpeed / (double)100)).ToString("F2");
      else
      {
         lbBallSpeed->Text = Math::Round(((RacquetData->usSpeed / 100) * ((double)36 / (double)10)), 2, System::MidpointRounding::AwayFromZero).ToString("F2");
      }
   }
}
void RacquetDisplay::btSendMetabolicData_Click(System::Object^  sender, System::EventArgs^  e) 
{
   RacquetData->ucMaxHeartRate = System::Convert::ToByte(nmMaxHR->Value);
   RacquetData->ucAvgHeartRate = System::Convert::ToByte(nmAvgHR->Value);
   RacquetData->usTotalCalories = System::Convert::ToUInt16(nmTotalCalories->Value);

   SendAckMessage(RacquetData->PAGE_METABOLIC_DATA);
}
void RacquetDisplay::btSendSpeedData_Click(System::Object^  sender, System::EventArgs^  e) 
{
   RacquetData->usMaxRunningSpeed = System::Convert::ToUInt16(nmMaxRunSpeed->Value );
   RacquetData->usAvgRunningSpeed = System::Convert::ToUInt16(nmAvgRunSpeed->Value);

   SendAckMessage(RacquetData->PAGE_SPEED_DATA);
}
void RacquetDisplay::btSendDistanceData_Click(System::Object^  sender, System::EventArgs^  e) 
{
   RacquetData->uiTotalDistance = System::Convert::ToUInt32(nmTotalDistance->Value);

   SendAckMessage(RacquetData->PAGE_DISTANCE_DATA);
}