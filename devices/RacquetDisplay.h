/*
This software is subject to the license described in the License.txt file 
included with this software distribution. You may not use this file except in compliance 
with this license.

Copyright (c) Dynastream Innovations Inc. 2012
All rights reserved.
*/
 

#pragma once
#include "StdAfx.h"
#include "ISimBase.h"
#include "antplus_racquet.h"	// Device specific class, if used
#include "antplus_common.h"		// Include common pages (example usage on SDM and Bike Power)
#include "types.h"
#include "antdefines.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class RacquetDisplay : public System::Windows::Forms::Form, public ISimBase{
public:
	// The constructor may include the following parameters:
	// System::Timers::Timer^ channelTimer:  Timer handle, if using a timer (not required in most cases, since getTimerInterval gets called after onTimerTock on each tock)
	// dRequestAckMsg^ channelAckMsg:  Handle to delegate function to send acknowledged messages (only needed if the device needs to send acknowledged messages)
	// These parameters may or may not be included in the constructor, as needed
		RacquetDisplay(System::Timers::Timer^ channelTimer, dRequestAckMsg^ channelAckMsg, dRequestBcastMsg^ channelBcastMsg){
			InitializeComponent();
			RacquetData = gcnew AntPlusRacquet();
			Common = gcnew CommonData();
			requestAckMsg = channelAckMsg;
			requestBcastMsg = channelBcastMsg;
			timerHandle = channelTimer;		
			InitializeSim();
		}

		~RacquetDisplay(){
			this->panel_Display->Controls->Clear();
			this->panel_Settings->Controls->Clear();
			delete this->panel_Display;
			delete this->panel_Settings;
			//clean up floating resources with the garbage collector
			GC::Collect(2);

			if (components)
			{
				delete components;
			}
		}

// Methods required by the interface class
public:
	virtual void onTimerTock(USHORT eventTime);							// Called every simulator event.  Empty function for most receivers (timer is disabled on receivers unless explicitly enabled)
	virtual void ANT_eventNotification(UCHAR ucEventCode_, UCHAR* pucEventBuffer_);	 // Channel event state machine
	virtual UCHAR getDeviceType(){return AntPlusRacquet::DEVICE_TYPE;}  	
	virtual UCHAR getTransmissionType(){return 0;} 	
	virtual USHORT getTransmitPeriod(){return AntPlusRacquet::MSG_PERIOD;}	
	virtual DOUBLE getTimerInterval(){return (double) ulTimerInterval;}	// Set to any value for receivers not using the timer.
	virtual System::Windows::Forms::Panel^ getSimSettingsPanel(){return this->panel_Settings;}
	virtual System::Windows::Forms::Panel^ getSimTranslatedDisplay(){return this->panel_Display;}

// Optional methods (not required by interface class)
private:
	void InitializeSim();						// Initialize simulation
	void UpdateDisplay(UCHAR ucPageNum_);
	void UpdateDisplayAckStatus(UCHAR ucStatus_);
	void SendAckMessage(UCHAR ucMsgCode_);
	BOOL HandleRetransmit();
	System::String^ GetStrokeText(UCHAR ucRacquetData_, UCHAR ucPageNum_);
	System::String^ GetZoneText(UCHAR ucRacquetData_, UCHAR ucPageNum_);
	System::String^ GetANTFSText (UCHAR ucRacquetData_, UCHAR ucPageNum_);
	System::Void btRequestEvent_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void btRequestStrokeCount_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void btRequestZoneCount_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void btSessionStart_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void btSessionStop_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void btMarkStroke_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void btSendGameScore_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void btSendSessionData_Click(System::Object^  sender, System::EventArgs^  e); 
	System::Void btSendSetCount_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void btSendGameCount_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void cbInvalidMyGameCount_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbInvalidOpponentGameCount_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbInvalidMySetCount_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbInvalidOpponentSetCount_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbInvalidMySetsWon_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbInvalidOpponentSetsWon_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbInvalidMyGameScore_CheckedChanged(System::Object ^sender, System::EventArgs ^e);
	System::Void cbInvalidOpponentGameScore_CheckedChanged(System::Object ^sender, System::EventArgs ^e);
   System::Void rbMPS_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
   System::Void btTimerStart_Click(System::Object^  sender, System::EventArgs^  e);
   System::Void btTimerStop_Click(System::Object^  sender, System::EventArgs^  e);
   System::Void btSendMetabolicData_Click(System::Object^  sender, System::EventArgs^  e);
   System::Void btSendSpeedData_Click(System::Object^  sender, System::EventArgs^  e);
   System::Void btSendDistanceData_Click(System::Object^  sender, System::EventArgs^  e);

private:
	dRequestAckMsg^ requestAckMsg;		// Delegate handle (ACK messages, only if used)
	dRequestBcastMsg^ requestBcastMsg;	
	System::Timers::Timer^ timerHandle;	// Timer handle (only if there is a timer)
	ULONG  ulTimerInterval;				// Timer interval between simulated events (only if there is a timer)
	
	
	// Racquet Object
	AntPlusRacquet^ RacquetData;
	CommonData^ Common;

	UCHAR ucAckRetryCount;
	UCHAR ucMsgExpectingAck;

private: System::Windows::Forms::Panel^  panel_Settings;
private: System::Windows::Forms::Panel^  panel_Display;
private: System::Windows::Forms::TabControl^  tabControl1;
private: System::Windows::Forms::TabPage^  tabPage1;
private: System::Windows::Forms::TabPage^  tabPage2;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::TabPage^  tabPage3;
private: System::Windows::Forms::Label^  lbBallSpeed;
private: System::Windows::Forms::Label^  lbRacquetZone;
private: System::Windows::Forms::Label^  lbStrokeType;
private: System::Windows::Forms::Label^  lbEventTime;
private: System::Windows::Forms::Label^  lbEventCount;
private: System::Windows::Forms::TabControl^  tabControl2;
private: System::Windows::Forms::TabPage^  tabPage4;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::Button^  btRequestStrokeCount;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::ComboBox^  cbStrokeType1;
private: System::Windows::Forms::ComboBox^  cbStrokeType2;
private: System::Windows::Forms::TabPage^  tabPage5;
private: System::Windows::Forms::Label^  lbZoneCount1;
private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::Label^  label18;
private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::ComboBox^  cbZoneType;
private: System::Windows::Forms::ComboBox^  cbStrokeType;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Button^  btRequestZoneCount;
private: System::Windows::Forms::Label^  label30;
private: System::Windows::Forms::Label^  lbPreviousSpeed;
private: System::Windows::Forms::Label^  lbPreviousZone;
private: System::Windows::Forms::Label^  lbPreviousTime;
private: System::Windows::Forms::Label^  lbPreviousCount;
private: System::Windows::Forms::Label^  lbPreviousType;
private: System::Windows::Forms::Label^  label24;
private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::Label^  label27;
private: System::Windows::Forms::Label^  label28;
private: System::Windows::Forms::Label^  label29;
private: System::Windows::Forms::NumericUpDown^  nmPreviousEvent;
private: System::Windows::Forms::Button^  btRequestEvent;
private: System::Windows::Forms::Label^  label11;
private: System::Windows::Forms::NumericUpDown^  nmTxResponseStrokes;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::Label^  lbStrokeCount1;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  nmTxResponse;
private: System::Windows::Forms::NumericUpDown^  nmTxResponseZones;
private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::NumericUpDown^  nmTxResponsePrevious;
private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::Label^  lbStrokeCount2;
private: System::Windows::Forms::Label^  lbStrokeType2;
private: System::Windows::Forms::Label^  lbStrokeType1;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::Label^  lbZoneCount2;
private: System::Windows::Forms::Label^  lbZone2;
private: System::Windows::Forms::Label^  lbZone1;
private: System::Windows::Forms::Label^  lbZoneStroke;
private: System::Windows::Forms::Label^  label23;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::TabPage^  tabPage6;
private: System::Windows::Forms::Label^  label_Glb_SoftwareVer;
private: System::Windows::Forms::Label^  label_Glb_HardwareVer;
private: System::Windows::Forms::Label^  lbSoftwareVer;
private: System::Windows::Forms::Label^  lbSerialNumber;
private: System::Windows::Forms::Label^  label_Glb_ModelNum;
private: System::Windows::Forms::Label^  lbManfId;
private: System::Windows::Forms::Label^  label_Glb_SerialNum;
private: System::Windows::Forms::Label^  lbModelNumber;
private: System::Windows::Forms::Label^  lbHardwareVer;
private: System::Windows::Forms::Label^  label_Glb_ManfID;
private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::Label^  lbAckStatus;
private: System::Windows::Forms::Label^  label33;
private: System::Windows::Forms::Label^  lbDisplayStroke;
private: System::Windows::Forms::Label^  lbDisplaySpeed;
private: System::Windows::Forms::Label^  lbDisplayEvent;
private: System::Windows::Forms::Label^  lbDisplayTime;
private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::Label^  lbDisplayZone;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::Label^  label31;
private: System::Windows::Forms::Label^  label32;
private: System::Windows::Forms::TabPage^  tabPage7;
private: System::Windows::Forms::TabControl^  tabControl3;
private: System::Windows::Forms::TabPage^  tabPage8;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::TabPage^  tabPage9;
private: System::Windows::Forms::Button^  btSessionStop;
private: System::Windows::Forms::Button^  btSessionStart;
private: System::Windows::Forms::Label^  label35;
private: System::Windows::Forms::Label^  label34;
private: System::Windows::Forms::GroupBox^  groupBox2;
private: System::Windows::Forms::Label^  label37;
private: System::Windows::Forms::Label^  label36;
private: System::Windows::Forms::Label^  label38;
private: System::Windows::Forms::NumericUpDown^  nmOpponentGameScore;
private: System::Windows::Forms::NumericUpDown^  nmMyGameScore;
private: System::Windows::Forms::CheckBox^ cbInvalidMyGameScore;
private: System::Windows::Forms::CheckBox^ cbInvalidOpponentGameScore;
private: System::Windows::Forms::Button^  btSendGameScore;
private: System::Windows::Forms::Button^  btMarkStroke;
private: System::Windows::Forms::Button^  btSendSessionData;
private: System::Windows::Forms::ComboBox^  cbGameMode;
private: System::Windows::Forms::CheckBox^  cbInvalidOpponentSetsWon;
private: System::Windows::Forms::CheckBox^  cbInvalidMySetsWon;
private: System::Windows::Forms::NumericUpDown^  nmOpponentSetsWon;
private: System::Windows::Forms::NumericUpDown^  nmMySetsWon;
private: System::Windows::Forms::TabPage^  tabPage10;
private: System::Windows::Forms::Label^  label39;
private: System::Windows::Forms::Label^  label41;
private: System::Windows::Forms::Label^  label40;
private: System::Windows::Forms::TabPage^  tabPage11;
private: System::Windows::Forms::Label^  label42;
private: System::Windows::Forms::Label^  label43;
private: System::Windows::Forms::Label^  label44;
private: System::Windows::Forms::CheckBox^  cbInvalidOpponentSetCount;
private: System::Windows::Forms::CheckBox^  cbInvalidMySetCount;
private: System::Windows::Forms::Button^  btSendSetCount;
private: System::Windows::Forms::NumericUpDown^  nmMySetCount;
private: System::Windows::Forms::NumericUpDown^  nmOpponentSetCount;
private: System::Windows::Forms::CheckBox^  cbInvalidOpponentGameCount;
private: System::Windows::Forms::CheckBox^  cbInvalidMyGameCount;
private: System::Windows::Forms::Button^  btSendGameCount;
private: System::Windows::Forms::NumericUpDown^  nmMyGameCount;
private: System::Windows::Forms::NumericUpDown^  nmOpponentGameCount;
private: System::Windows::Forms::GroupBox^  groupBox4;
private: System::Windows::Forms::GroupBox^  groupBox3;
private: System::Windows::Forms::RadioButton^  rbMPS;
private: System::Windows::Forms::RadioButton^  rbKMPH;
private: System::Windows::Forms::GroupBox^  groupBox5;
private: System::Windows::Forms::Button^  btTimerStop;
private: System::Windows::Forms::Button^  btTimerStart;
private: System::Windows::Forms::TabControl^  tabControl4;
private: System::Windows::Forms::TabPage^  tabPage12;
private: System::Windows::Forms::TabPage^  tabPage13;
private: System::Windows::Forms::Label^  label45;
private: System::Windows::Forms::Label^  labelTimeRes;
private: System::Windows::Forms::Label^  label46;
private: System::Windows::Forms::Label^  labelOpTime;
private: System::Windows::Forms::Label^  label47;
private: System::Windows::Forms::Label^  labelBattVolt;
private: System::Windows::Forms::Label^  label48;
private: System::Windows::Forms::Label^  labelBattStatus;


































private: System::Windows::Forms::TabPage^  tabPage17;
private: System::Windows::Forms::TabControl^  tabControl5;
private: System::Windows::Forms::TabPage^  tabPage18;
private: System::Windows::Forms::TabPage^  tabPage19;
private: System::Windows::Forms::Button^  btSendMetabolicData;
private: System::Windows::Forms::GroupBox^  groupBox_Calories;
private: System::Windows::Forms::Label^  label52;
private: System::Windows::Forms::NumericUpDown^  nmTotalCalories;
private: System::Windows::Forms::Label^  label51;
private: System::Windows::Forms::GroupBox^  groupBox_HR;
private: System::Windows::Forms::Label^  label54;
private: System::Windows::Forms::Label^  label53;
private: System::Windows::Forms::NumericUpDown^  nmAvgHR;
private: System::Windows::Forms::NumericUpDown^  nmMaxHR;
private: System::Windows::Forms::Label^  label50;
private: System::Windows::Forms::Label^  label49;
private: System::Windows::Forms::Label^  label59;
private: System::Windows::Forms::Label^  label58;
private: System::Windows::Forms::Label^  label55;
private: System::Windows::Forms::Label^  label56;
private: System::Windows::Forms::Label^  label57;
private: System::Windows::Forms::Button^  btSendSpeedData;
private: System::Windows::Forms::NumericUpDown^  nmMaxRunSpeed;
private: System::Windows::Forms::NumericUpDown^  nmAvgRunSpeed;
private: System::Windows::Forms::TabPage^  tabPage20;
private: System::Windows::Forms::Label^  label60;
private: System::Windows::Forms::Label^  label61;
private: System::Windows::Forms::Button^  btSendDistanceData;
private: System::Windows::Forms::NumericUpDown^  nmTotalDistance;
private: System::Windows::Forms::Label^  label62;
private: System::Windows::Forms::Label^  lbANTFS;











		 /// <summary>
		/// Required designer variable.a
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel_Settings = (gcnew System::Windows::Forms::Panel());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->lbANTFS = (gcnew System::Windows::Forms::Label());
			this->label62 = (gcnew System::Windows::Forms::Label());
			this->rbKMPH = (gcnew System::Windows::Forms::RadioButton());
			this->rbMPS = (gcnew System::Windows::Forms::RadioButton());
			this->lbBallSpeed = (gcnew System::Windows::Forms::Label());
			this->lbRacquetZone = (gcnew System::Windows::Forms::Label());
			this->lbStrokeType = (gcnew System::Windows::Forms::Label());
			this->lbEventTime = (gcnew System::Windows::Forms::Label());
			this->lbEventCount = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl2 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->lbStrokeCount2 = (gcnew System::Windows::Forms::Label());
			this->lbStrokeType2 = (gcnew System::Windows::Forms::Label());
			this->lbStrokeType1 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->nmTxResponseStrokes = (gcnew System::Windows::Forms::NumericUpDown());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->lbStrokeCount1 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->btRequestStrokeCount = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->cbStrokeType1 = (gcnew System::Windows::Forms::ComboBox());
			this->cbStrokeType2 = (gcnew System::Windows::Forms::ComboBox());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->lbZoneCount2 = (gcnew System::Windows::Forms::Label());
			this->lbZone2 = (gcnew System::Windows::Forms::Label());
			this->lbZone1 = (gcnew System::Windows::Forms::Label());
			this->lbZoneStroke = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->nmTxResponse = (gcnew System::Windows::Forms::Label());
			this->nmTxResponseZones = (gcnew System::Windows::Forms::NumericUpDown());
			this->lbZoneCount1 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->cbZoneType = (gcnew System::Windows::Forms::ComboBox());
			this->cbStrokeType = (gcnew System::Windows::Forms::ComboBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->btRequestZoneCount = (gcnew System::Windows::Forms::Button());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->nmTxResponsePrevious = (gcnew System::Windows::Forms::NumericUpDown());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->nmPreviousEvent = (gcnew System::Windows::Forms::NumericUpDown());
			this->btRequestEvent = (gcnew System::Windows::Forms::Button());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->lbPreviousSpeed = (gcnew System::Windows::Forms::Label());
			this->lbPreviousZone = (gcnew System::Windows::Forms::Label());
			this->lbPreviousTime = (gcnew System::Windows::Forms::Label());
			this->lbPreviousCount = (gcnew System::Windows::Forms::Label());
			this->lbPreviousType = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->tabPage7 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl3 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage8 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->btTimerStop = (gcnew System::Windows::Forms::Button());
			this->btTimerStart = (gcnew System::Windows::Forms::Button());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->nmOpponentGameScore = (gcnew System::Windows::Forms::NumericUpDown());
			this->nmMyGameScore = (gcnew System::Windows::Forms::NumericUpDown());
			this->cbInvalidMyGameScore = (gcnew System::Windows::Forms::CheckBox());
			this->cbInvalidOpponentGameScore = (gcnew System::Windows::Forms::CheckBox());
			this->btSendGameScore = (gcnew System::Windows::Forms::Button());
			this->btMarkStroke = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->btSessionStop = (gcnew System::Windows::Forms::Button());
			this->btSessionStart = (gcnew System::Windows::Forms::Button());
			this->tabPage9 = (gcnew System::Windows::Forms::TabPage());
			this->btSendSessionData = (gcnew System::Windows::Forms::Button());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->cbGameMode = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->cbInvalidOpponentSetsWon = (gcnew System::Windows::Forms::CheckBox());
			this->cbInvalidMySetsWon = (gcnew System::Windows::Forms::CheckBox());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->nmOpponentSetsWon = (gcnew System::Windows::Forms::NumericUpDown());
			this->nmMySetsWon = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabPage10 = (gcnew System::Windows::Forms::TabPage());
			this->cbInvalidOpponentSetCount = (gcnew System::Windows::Forms::CheckBox());
			this->cbInvalidMySetCount = (gcnew System::Windows::Forms::CheckBox());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->btSendSetCount = (gcnew System::Windows::Forms::Button());
			this->nmMySetCount = (gcnew System::Windows::Forms::NumericUpDown());
			this->nmOpponentSetCount = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabPage11 = (gcnew System::Windows::Forms::TabPage());
			this->cbInvalidOpponentGameCount = (gcnew System::Windows::Forms::CheckBox());
			this->cbInvalidMyGameCount = (gcnew System::Windows::Forms::CheckBox());
			this->label42 = (gcnew System::Windows::Forms::Label());
			this->label43 = (gcnew System::Windows::Forms::Label());
			this->label44 = (gcnew System::Windows::Forms::Label());
			this->btSendGameCount = (gcnew System::Windows::Forms::Button());
			this->nmMyGameCount = (gcnew System::Windows::Forms::NumericUpDown());
			this->nmOpponentGameCount = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabPage17 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl5 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage18 = (gcnew System::Windows::Forms::TabPage());
			this->btSendMetabolicData = (gcnew System::Windows::Forms::Button());
			this->groupBox_Calories = (gcnew System::Windows::Forms::GroupBox());
			this->label52 = (gcnew System::Windows::Forms::Label());
			this->nmTotalCalories = (gcnew System::Windows::Forms::NumericUpDown());
			this->label51 = (gcnew System::Windows::Forms::Label());
			this->groupBox_HR = (gcnew System::Windows::Forms::GroupBox());
			this->label54 = (gcnew System::Windows::Forms::Label());
			this->label53 = (gcnew System::Windows::Forms::Label());
			this->nmAvgHR = (gcnew System::Windows::Forms::NumericUpDown());
			this->nmMaxHR = (gcnew System::Windows::Forms::NumericUpDown());
			this->label50 = (gcnew System::Windows::Forms::Label());
			this->label49 = (gcnew System::Windows::Forms::Label());
			this->tabPage19 = (gcnew System::Windows::Forms::TabPage());
			this->label59 = (gcnew System::Windows::Forms::Label());
			this->label58 = (gcnew System::Windows::Forms::Label());
			this->label55 = (gcnew System::Windows::Forms::Label());
			this->label56 = (gcnew System::Windows::Forms::Label());
			this->label57 = (gcnew System::Windows::Forms::Label());
			this->btSendSpeedData = (gcnew System::Windows::Forms::Button());
			this->nmMaxRunSpeed = (gcnew System::Windows::Forms::NumericUpDown());
			this->nmAvgRunSpeed = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabPage20 = (gcnew System::Windows::Forms::TabPage());
			this->label60 = (gcnew System::Windows::Forms::Label());
			this->label61 = (gcnew System::Windows::Forms::Label());
			this->btSendDistanceData = (gcnew System::Windows::Forms::Button());
			this->nmTotalDistance = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabPage6 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl4 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage12 = (gcnew System::Windows::Forms::TabPage());
			this->label_Glb_SoftwareVer = (gcnew System::Windows::Forms::Label());
			this->label_Glb_SerialNum = (gcnew System::Windows::Forms::Label());
			this->lbSerialNumber = (gcnew System::Windows::Forms::Label());
			this->lbSoftwareVer = (gcnew System::Windows::Forms::Label());
			this->label_Glb_HardwareVer = (gcnew System::Windows::Forms::Label());
			this->label_Glb_ManfID = (gcnew System::Windows::Forms::Label());
			this->lbModelNumber = (gcnew System::Windows::Forms::Label());
			this->lbHardwareVer = (gcnew System::Windows::Forms::Label());
			this->lbManfId = (gcnew System::Windows::Forms::Label());
			this->label_Glb_ModelNum = (gcnew System::Windows::Forms::Label());
			this->tabPage13 = (gcnew System::Windows::Forms::TabPage());
			this->label45 = (gcnew System::Windows::Forms::Label());
			this->labelTimeRes = (gcnew System::Windows::Forms::Label());
			this->label46 = (gcnew System::Windows::Forms::Label());
			this->labelOpTime = (gcnew System::Windows::Forms::Label());
			this->label47 = (gcnew System::Windows::Forms::Label());
			this->labelBattVolt = (gcnew System::Windows::Forms::Label());
			this->label48 = (gcnew System::Windows::Forms::Label());
			this->labelBattStatus = (gcnew System::Windows::Forms::Label());
			this->panel_Display = (gcnew System::Windows::Forms::Panel());
			this->lbAckStatus = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->lbDisplayStroke = (gcnew System::Windows::Forms::Label());
			this->lbDisplaySpeed = (gcnew System::Windows::Forms::Label());
			this->lbDisplayEvent = (gcnew System::Windows::Forms::Label());
			this->lbDisplayTime = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->lbDisplayZone = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->panel_Settings->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabControl2->SuspendLayout();
			this->tabPage4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTxResponseStrokes))->BeginInit();
			this->tabPage5->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTxResponseZones))->BeginInit();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTxResponsePrevious))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmPreviousEvent))->BeginInit();
			this->tabPage7->SuspendLayout();
			this->tabControl3->SuspendLayout();
			this->tabPage8->SuspendLayout();
			this->groupBox5->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmOpponentGameScore))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMyGameScore))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->tabPage9->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmOpponentSetsWon))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMySetsWon))->BeginInit();
			this->tabPage10->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMySetCount))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmOpponentSetCount))->BeginInit();
			this->tabPage11->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMyGameCount))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmOpponentGameCount))->BeginInit();
			this->tabPage17->SuspendLayout();
			this->tabControl5->SuspendLayout();
			this->tabPage18->SuspendLayout();
			this->groupBox_Calories->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTotalCalories))->BeginInit();
			this->groupBox_HR->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmAvgHR))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMaxHR))->BeginInit();
			this->tabPage19->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMaxRunSpeed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmAvgRunSpeed))->BeginInit();
			this->tabPage20->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTotalDistance))->BeginInit();
			this->tabPage6->SuspendLayout();
			this->tabControl4->SuspendLayout();
			this->tabPage12->SuspendLayout();
			this->tabPage13->SuspendLayout();
			this->panel_Display->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel_Settings
			// 
			this->panel_Settings->Controls->Add(this->tabControl1);
			this->panel_Settings->Location = System::Drawing::Point(322, 50);
			this->panel_Settings->Name = L"panel_Settings";
			this->panel_Settings->Size = System::Drawing::Size(400, 140);
			this->panel_Settings->TabIndex = 0;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Controls->Add(this->tabPage7);
			this->tabControl1->Controls->Add(this->tabPage17);
			this->tabControl1->Controls->Add(this->tabPage6);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(400, 140);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->lbANTFS);
			this->tabPage1->Controls->Add(this->label62);
			this->tabPage1->Controls->Add(this->rbKMPH);
			this->tabPage1->Controls->Add(this->rbMPS);
			this->tabPage1->Controls->Add(this->lbBallSpeed);
			this->tabPage1->Controls->Add(this->lbRacquetZone);
			this->tabPage1->Controls->Add(this->lbStrokeType);
			this->tabPage1->Controls->Add(this->lbEventTime);
			this->tabPage1->Controls->Add(this->lbEventCount);
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Margin = System::Windows::Forms::Padding(5);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(392, 114);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Current Event";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// lbANTFS
			// 
			this->lbANTFS->AutoSize = true;
			this->lbANTFS->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->lbANTFS->Location = System::Drawing::Point(117, 88);
			this->lbANTFS->Name = L"lbANTFS";
			this->lbANTFS->Size = System::Drawing::Size(19, 13);
			this->lbANTFS->TabIndex = 13;
			this->lbANTFS->Text = L"----";
			// 
			// label62
			// 
			this->label62->AutoSize = true;
			this->label62->Location = System::Drawing::Point(9, 88);
			this->label62->Name = L"label62";
			this->label62->Size = System::Drawing::Size(100, 13);
			this->label62->TabIndex = 12;
			this->label62->Text = L"ANT-FS Supported:";
			// 
			// rbKMPH
			// 
			this->rbKMPH->AutoSize = true;
			this->rbKMPH->Location = System::Drawing::Point(278, 67);
			this->rbKMPH->Name = L"rbKMPH";
			this->rbKMPH->Size = System::Drawing::Size(50, 17);
			this->rbKMPH->TabIndex = 11;
			this->rbKMPH->Text = L"km/h";
			this->rbKMPH->UseVisualStyleBackColor = true;
			// 
			// rbMPS
			// 
			this->rbMPS->AutoSize = true;
			this->rbMPS->Checked = true;
			this->rbMPS->Location = System::Drawing::Point(229, 67);
			this->rbMPS->Name = L"rbMPS";
			this->rbMPS->Size = System::Drawing::Size(43, 17);
			this->rbMPS->TabIndex = 10;
			this->rbMPS->TabStop = true;
			this->rbMPS->Text = L"m/s";
			this->rbMPS->UseVisualStyleBackColor = true;
			this->rbMPS->CheckedChanged += gcnew System::EventHandler(this, &RacquetDisplay::rbMPS_CheckedChanged);
			// 
			// lbBallSpeed
			// 
			this->lbBallSpeed->AutoSize = true;
			this->lbBallSpeed->Location = System::Drawing::Point(266, 44);
			this->lbBallSpeed->Name = L"lbBallSpeed";
			this->lbBallSpeed->Size = System::Drawing::Size(19, 13);
			this->lbBallSpeed->TabIndex = 9;
			this->lbBallSpeed->Text = L"----";
			// 
			// lbRacquetZone
			// 
			this->lbRacquetZone->AutoSize = true;
			this->lbRacquetZone->Location = System::Drawing::Point(266, 21);
			this->lbRacquetZone->Name = L"lbRacquetZone";
			this->lbRacquetZone->Size = System::Drawing::Size(19, 13);
			this->lbRacquetZone->TabIndex = 8;
			this->lbRacquetZone->Text = L"----";
			// 
			// lbStrokeType
			// 
			this->lbStrokeType->AutoSize = true;
			this->lbStrokeType->Location = System::Drawing::Point(117, 67);
			this->lbStrokeType->Name = L"lbStrokeType";
			this->lbStrokeType->Size = System::Drawing::Size(19, 13);
			this->lbStrokeType->TabIndex = 7;
			this->lbStrokeType->Text = L"----";
			// 
			// lbEventTime
			// 
			this->lbEventTime->AutoSize = true;
			this->lbEventTime->Location = System::Drawing::Point(117, 44);
			this->lbEventTime->Name = L"lbEventTime";
			this->lbEventTime->Size = System::Drawing::Size(19, 13);
			this->lbEventTime->TabIndex = 6;
			this->lbEventTime->Text = L"----";
			// 
			// lbEventCount
			// 
			this->lbEventCount->AutoSize = true;
			this->lbEventCount->Location = System::Drawing::Point(117, 21);
			this->lbEventCount->Name = L"lbEventCount";
			this->lbEventCount->Size = System::Drawing::Size(19, 13);
			this->lbEventCount->TabIndex = 5;
			this->lbEventCount->Text = L"----";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(197, 44);
			this->label5->Margin = System::Windows::Forms::Padding(5);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(61, 13);
			this->label5->TabIndex = 4;
			this->label5->Text = L"Ball Speed:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(179, 21);
			this->label4->Margin = System::Windows::Forms::Padding(5);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(79, 13);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Racquet Zone:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(44, 67);
			this->label3->Margin = System::Windows::Forms::Padding(5);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(65, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Stoke Type:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(44, 44);
			this->label2->Margin = System::Windows::Forms::Padding(5);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(64, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Event Time:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 21);
			this->label1->Margin = System::Windows::Forms::Padding(5);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(103, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Stroke Event Count:";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->tabControl2);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(392, 114);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Stroke Summary";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// tabControl2
			// 
			this->tabControl2->Controls->Add(this->tabPage4);
			this->tabControl2->Controls->Add(this->tabPage5);
			this->tabControl2->Location = System::Drawing::Point(3, 3);
			this->tabControl2->Name = L"tabControl2";
			this->tabControl2->SelectedIndex = 0;
			this->tabControl2->Size = System::Drawing::Size(383, 105);
			this->tabControl2->TabIndex = 0;
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->lbStrokeCount2);
			this->tabPage4->Controls->Add(this->lbStrokeType2);
			this->tabPage4->Controls->Add(this->lbStrokeType1);
			this->tabPage4->Controls->Add(this->label17);
			this->tabPage4->Controls->Add(this->label14);
			this->tabPage4->Controls->Add(this->label11);
			this->tabPage4->Controls->Add(this->nmTxResponseStrokes);
			this->tabPage4->Controls->Add(this->label10);
			this->tabPage4->Controls->Add(this->lbStrokeCount1);
			this->tabPage4->Controls->Add(this->label6);
			this->tabPage4->Controls->Add(this->label9);
			this->tabPage4->Controls->Add(this->btRequestStrokeCount);
			this->tabPage4->Controls->Add(this->label8);
			this->tabPage4->Controls->Add(this->cbStrokeType1);
			this->tabPage4->Controls->Add(this->cbStrokeType2);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(375, 79);
			this->tabPage4->TabIndex = 0;
			this->tabPage4->Text = L"By Stroke Type";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// lbStrokeCount2
			// 
			this->lbStrokeCount2->AutoSize = true;
			this->lbStrokeCount2->Location = System::Drawing::Point(294, 58);
			this->lbStrokeCount2->Name = L"lbStrokeCount2";
			this->lbStrokeCount2->Size = System::Drawing::Size(19, 13);
			this->lbStrokeCount2->TabIndex = 35;
			this->lbStrokeCount2->Text = L"----";
			// 
			// lbStrokeType2
			// 
			this->lbStrokeType2->AutoSize = true;
			this->lbStrokeType2->Location = System::Drawing::Point(294, 41);
			this->lbStrokeType2->Name = L"lbStrokeType2";
			this->lbStrokeType2->Size = System::Drawing::Size(19, 13);
			this->lbStrokeType2->TabIndex = 34;
			this->lbStrokeType2->Text = L"----";
			// 
			// lbStrokeType1
			// 
			this->lbStrokeType1->AutoSize = true;
			this->lbStrokeType1->Location = System::Drawing::Point(294, 7);
			this->lbStrokeType1->Name = L"lbStrokeType1";
			this->lbStrokeType1->Size = System::Drawing::Size(19, 13);
			this->lbStrokeType1->TabIndex = 33;
			this->lbStrokeType1->Text = L"----";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(250, 58);
			this->label17->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(38, 13);
			this->label17->TabIndex = 32;
			this->label17->Text = L"Count:";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(241, 41);
			this->label14->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(47, 13);
			this->label14->TabIndex = 31;
			this->label14->Text = L"Stroke2:";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(164, 37);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(70, 13);
			this->label11->TabIndex = 29;
			this->label11->Text = L"Tx Response";
			// 
			// nmTxResponseStrokes
			// 
			this->nmTxResponseStrokes->Location = System::Drawing::Point(167, 53);
			this->nmTxResponseStrokes->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {127, 0, 0, 0});
			this->nmTxResponseStrokes->Name = L"nmTxResponseStrokes";
			this->nmTxResponseStrokes->Size = System::Drawing::Size(67, 20);
			this->nmTxResponseStrokes->TabIndex = 28;
			this->nmTxResponseStrokes->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {4, 0, 0, 0});
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(250, 24);
			this->label10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(38, 13);
			this->label10->TabIndex = 27;
			this->label10->Text = L"Count:";
			// 
			// lbStrokeCount1
			// 
			this->lbStrokeCount1->AutoSize = true;
			this->lbStrokeCount1->Location = System::Drawing::Point(294, 24);
			this->lbStrokeCount1->Name = L"lbStrokeCount1";
			this->lbStrokeCount1->Size = System::Drawing::Size(19, 13);
			this->lbStrokeCount1->TabIndex = 26;
			this->lbStrokeCount1->Text = L"----";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(241, 7);
			this->label6->Margin = System::Windows::Forms::Padding(3, 0, 3, 2);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(47, 13);
			this->label6->TabIndex = 24;
			this->label6->Text = L"Stroke1:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(6, 32);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(74, 13);
			this->label9->TabIndex = 6;
			this->label9->Text = L"Stroke Type 2";
			// 
			// btRequestStrokeCount
			// 
			this->btRequestStrokeCount->Location = System::Drawing::Point(9, 52);
			this->btRequestStrokeCount->Margin = System::Windows::Forms::Padding(3, 1, 3, 3);
			this->btRequestStrokeCount->Name = L"btRequestStrokeCount";
			this->btRequestStrokeCount->Size = System::Drawing::Size(152, 23);
			this->btRequestStrokeCount->TabIndex = 0;
			this->btRequestStrokeCount->Text = L"Request Stroke Count";
			this->btRequestStrokeCount->UseVisualStyleBackColor = true;
			this->btRequestStrokeCount->Click += gcnew System::EventHandler(this, &RacquetDisplay::btRequestStrokeCount_Click);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(6, 9);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(74, 13);
			this->label8->TabIndex = 5;
			this->label8->Text = L"Stroke Type 1";
			// 
			// cbStrokeType1
			// 
			this->cbStrokeType1->FormattingEnabled = true;
			this->cbStrokeType1->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"All Strokes", L"Other", L"Serve", L"Forehand", 
				L"Backhand", L"Smash", L"Unknown"});
			this->cbStrokeType1->Location = System::Drawing::Point(86, 6);
			this->cbStrokeType1->Margin = System::Windows::Forms::Padding(3, 3, 3, 1);
			this->cbStrokeType1->Name = L"cbStrokeType1";
			this->cbStrokeType1->Size = System::Drawing::Size(75, 21);
			this->cbStrokeType1->TabIndex = 1;
			this->cbStrokeType1->Text = L"Backhand";
			// 
			// cbStrokeType2
			// 
			this->cbStrokeType2->FormattingEnabled = true;
			this->cbStrokeType2->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"All Strokes", L"Other", L"Serve", L"Forehand", 
				L"Backhand", L"Smash", L"Unknown"});
			this->cbStrokeType2->Location = System::Drawing::Point(86, 29);
			this->cbStrokeType2->Margin = System::Windows::Forms::Padding(3, 1, 1, 1);
			this->cbStrokeType2->Name = L"cbStrokeType2";
			this->cbStrokeType2->Size = System::Drawing::Size(75, 21);
			this->cbStrokeType2->TabIndex = 2;
			this->cbStrokeType2->Text = L"Backhand";
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->lbZoneCount2);
			this->tabPage5->Controls->Add(this->lbZone2);
			this->tabPage5->Controls->Add(this->lbZone1);
			this->tabPage5->Controls->Add(this->lbZoneStroke);
			this->tabPage5->Controls->Add(this->label23);
			this->tabPage5->Controls->Add(this->label22);
			this->tabPage5->Controls->Add(this->nmTxResponse);
			this->tabPage5->Controls->Add(this->nmTxResponseZones);
			this->tabPage5->Controls->Add(this->lbZoneCount1);
			this->tabPage5->Controls->Add(this->label19);
			this->tabPage5->Controls->Add(this->label18);
			this->tabPage5->Controls->Add(this->label16);
			this->tabPage5->Controls->Add(this->cbZoneType);
			this->tabPage5->Controls->Add(this->cbStrokeType);
			this->tabPage5->Controls->Add(this->label15);
			this->tabPage5->Controls->Add(this->label7);
			this->tabPage5->Controls->Add(this->btRequestZoneCount);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Padding = System::Windows::Forms::Padding(3);
			this->tabPage5->Size = System::Drawing::Size(375, 79);
			this->tabPage5->TabIndex = 1;
			this->tabPage5->Text = L"By Racquet Zone";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// lbZoneCount2
			// 
			this->lbZoneCount2->AutoSize = true;
			this->lbZoneCount2->Location = System::Drawing::Point(287, 63);
			this->lbZoneCount2->Name = L"lbZoneCount2";
			this->lbZoneCount2->Size = System::Drawing::Size(19, 13);
			this->lbZoneCount2->TabIndex = 24;
			this->lbZoneCount2->Text = L"----";
			// 
			// lbZone2
			// 
			this->lbZone2->AutoSize = true;
			this->lbZone2->Location = System::Drawing::Point(287, 48);
			this->lbZone2->Name = L"lbZone2";
			this->lbZone2->Size = System::Drawing::Size(19, 13);
			this->lbZone2->TabIndex = 23;
			this->lbZone2->Text = L"----";
			// 
			// lbZone1
			// 
			this->lbZone1->AutoSize = true;
			this->lbZone1->Location = System::Drawing::Point(287, 18);
			this->lbZone1->Name = L"lbZone1";
			this->lbZone1->Size = System::Drawing::Size(19, 13);
			this->lbZone1->TabIndex = 22;
			this->lbZone1->Text = L"----";
			// 
			// lbZoneStroke
			// 
			this->lbZoneStroke->AutoSize = true;
			this->lbZoneStroke->Location = System::Drawing::Point(287, 3);
			this->lbZoneStroke->Name = L"lbZoneStroke";
			this->lbZoneStroke->Size = System::Drawing::Size(19, 13);
			this->lbZoneStroke->TabIndex = 21;
			this->lbZoneStroke->Text = L"----";
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(243, 63);
			this->label23->Margin = System::Windows::Forms::Padding(3, 1, 3, 1);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(38, 13);
			this->label23->TabIndex = 8;
			this->label23->Text = L"Count:";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(240, 48);
			this->label22->Margin = System::Windows::Forms::Padding(3, 1, 3, 1);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(41, 13);
			this->label22->TabIndex = 20;
			this->label22->Text = L"Zone2:";
			// 
			// nmTxResponse
			// 
			this->nmTxResponse->AutoSize = true;
			this->nmTxResponse->Location = System::Drawing::Point(151, 37);
			this->nmTxResponse->Name = L"nmTxResponse";
			this->nmTxResponse->Size = System::Drawing::Size(70, 13);
			this->nmTxResponse->TabIndex = 19;
			this->nmTxResponse->Text = L"Tx Response";
			// 
			// nmTxResponseZones
			// 
			this->nmTxResponseZones->Location = System::Drawing::Point(154, 53);
			this->nmTxResponseZones->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {127, 0, 0, 0});
			this->nmTxResponseZones->Name = L"nmTxResponseZones";
			this->nmTxResponseZones->Size = System::Drawing::Size(64, 20);
			this->nmTxResponseZones->TabIndex = 18;
			this->nmTxResponseZones->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {4, 0, 0, 0});
			// 
			// lbZoneCount1
			// 
			this->lbZoneCount1->AutoSize = true;
			this->lbZoneCount1->Location = System::Drawing::Point(287, 33);
			this->lbZoneCount1->Name = L"lbZoneCount1";
			this->lbZoneCount1->Size = System::Drawing::Size(19, 13);
			this->lbZoneCount1->TabIndex = 14;
			this->lbZoneCount1->Text = L"----";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(243, 33);
			this->label19->Margin = System::Windows::Forms::Padding(3, 1, 3, 1);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(38, 13);
			this->label19->TabIndex = 7;
			this->label19->Text = L"Count:";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(240, 3);
			this->label18->Margin = System::Windows::Forms::Padding(3, 0, 3, 1);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(41, 13);
			this->label18->TabIndex = 6;
			this->label18->Text = L"Stroke:";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(240, 18);
			this->label16->Margin = System::Windows::Forms::Padding(3, 1, 3, 1);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(41, 13);
			this->label16->TabIndex = 5;
			this->label16->Text = L"Zone1:";
			// 
			// cbZoneType
			// 
			this->cbZoneType->FormattingEnabled = true;
			this->cbZoneType->Items->AddRange(gcnew cli::array< System::Object^  >(12) {L"Miss", L"Top Right", L"Middle Right", L"Bottom Right", 
				L"Bottom Center", L"Bottom Left", L"Middle Center", L"Middle Left", L"Top Left", L"Top Center", L"Rim/Other", L"Unknown"});
			this->cbZoneType->Location = System::Drawing::Point(50, 29);
			this->cbZoneType->Margin = System::Windows::Forms::Padding(3, 1, 3, 1);
			this->cbZoneType->Name = L"cbZoneType";
			this->cbZoneType->Size = System::Drawing::Size(98, 21);
			this->cbZoneType->TabIndex = 4;
			this->cbZoneType->Text = L"Bottom Center";
			// 
			// cbStrokeType
			// 
			this->cbStrokeType->FormattingEnabled = true;
			this->cbStrokeType->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"All Events", L"Other", L"Serve", L"Forehand", 
				L"Backhand", L"Smash", L"Unknown"});
			this->cbStrokeType->Location = System::Drawing::Point(50, 4);
			this->cbStrokeType->Margin = System::Windows::Forms::Padding(3, 3, 3, 1);
			this->cbStrokeType->Name = L"cbStrokeType";
			this->cbStrokeType->Size = System::Drawing::Size(98, 21);
			this->cbStrokeType->TabIndex = 3;
			this->cbStrokeType->Text = L"Backhand";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(12, 32);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(32, 13);
			this->label15->TabIndex = 2;
			this->label15->Text = L"Zone";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(6, 7);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(38, 13);
			this->label7->TabIndex = 1;
			this->label7->Text = L"Stroke";
			// 
			// btRequestZoneCount
			// 
			this->btRequestZoneCount->Location = System::Drawing::Point(9, 52);
			this->btRequestZoneCount->Margin = System::Windows::Forms::Padding(3, 1, 3, 3);
			this->btRequestZoneCount->Name = L"btRequestZoneCount";
			this->btRequestZoneCount->Size = System::Drawing::Size(139, 23);
			this->btRequestZoneCount->TabIndex = 0;
			this->btRequestZoneCount->Text = L"Request Stroke Count";
			this->btRequestZoneCount->UseVisualStyleBackColor = true;
			this->btRequestZoneCount->Click += gcnew System::EventHandler(this, &RacquetDisplay::btRequestZoneCount_Click);
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->nmTxResponsePrevious);
			this->tabPage3->Controls->Add(this->label12);
			this->tabPage3->Controls->Add(this->nmPreviousEvent);
			this->tabPage3->Controls->Add(this->btRequestEvent);
			this->tabPage3->Controls->Add(this->label30);
			this->tabPage3->Controls->Add(this->lbPreviousSpeed);
			this->tabPage3->Controls->Add(this->lbPreviousZone);
			this->tabPage3->Controls->Add(this->lbPreviousTime);
			this->tabPage3->Controls->Add(this->lbPreviousCount);
			this->tabPage3->Controls->Add(this->lbPreviousType);
			this->tabPage3->Controls->Add(this->label24);
			this->tabPage3->Controls->Add(this->label25);
			this->tabPage3->Controls->Add(this->label27);
			this->tabPage3->Controls->Add(this->label28);
			this->tabPage3->Controls->Add(this->label29);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Size = System::Drawing::Size(392, 114);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Previous Event";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// nmTxResponsePrevious
			// 
			this->nmTxResponsePrevious->Location = System::Drawing::Point(97, 78);
			this->nmTxResponsePrevious->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {127, 0, 0, 0});
			this->nmTxResponsePrevious->Name = L"nmTxResponsePrevious";
			this->nmTxResponsePrevious->Size = System::Drawing::Size(67, 20);
			this->nmTxResponsePrevious->TabIndex = 26;
			this->nmTxResponsePrevious->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {4, 0, 0, 0});
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(21, 80);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(70, 13);
			this->label12->TabIndex = 25;
			this->label12->Text = L"Tx Response";
			// 
			// nmPreviousEvent
			// 
			this->nmPreviousEvent->Location = System::Drawing::Point(24, 23);
			this->nmPreviousEvent->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->nmPreviousEvent->Name = L"nmPreviousEvent";
			this->nmPreviousEvent->Size = System::Drawing::Size(101, 20);
			this->nmPreviousEvent->TabIndex = 23;
			this->nmPreviousEvent->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// btRequestEvent
			// 
			this->btRequestEvent->Location = System::Drawing::Point(23, 49);
			this->btRequestEvent->Name = L"btRequestEvent";
			this->btRequestEvent->Size = System::Drawing::Size(141, 23);
			this->btRequestEvent->TabIndex = 22;
			this->btRequestEvent->Text = L"Request Previous Event";
			this->btRequestEvent->UseVisualStyleBackColor = true;
			this->btRequestEvent->Click += gcnew System::EventHandler(this, &RacquetDisplay::btRequestEvent_Click);
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(21, 4);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(104, 13);
			this->label30->TabIndex = 21;
			this->label30->Text = L"Event # To Request";
			// 
			// lbPreviousSpeed
			// 
			this->lbPreviousSpeed->AutoSize = true;
			this->lbPreviousSpeed->Location = System::Drawing::Point(281, 86);
			this->lbPreviousSpeed->Name = L"lbPreviousSpeed";
			this->lbPreviousSpeed->Size = System::Drawing::Size(19, 13);
			this->lbPreviousSpeed->TabIndex = 19;
			this->lbPreviousSpeed->Text = L"----";
			// 
			// lbPreviousZone
			// 
			this->lbPreviousZone->AutoSize = true;
			this->lbPreviousZone->Location = System::Drawing::Point(281, 67);
			this->lbPreviousZone->Name = L"lbPreviousZone";
			this->lbPreviousZone->Size = System::Drawing::Size(19, 13);
			this->lbPreviousZone->TabIndex = 18;
			this->lbPreviousZone->Text = L"----";
			// 
			// lbPreviousTime
			// 
			this->lbPreviousTime->AutoSize = true;
			this->lbPreviousTime->Location = System::Drawing::Point(281, 46);
			this->lbPreviousTime->Name = L"lbPreviousTime";
			this->lbPreviousTime->Size = System::Drawing::Size(19, 13);
			this->lbPreviousTime->TabIndex = 17;
			this->lbPreviousTime->Text = L"----";
			// 
			// lbPreviousCount
			// 
			this->lbPreviousCount->AutoSize = true;
			this->lbPreviousCount->Location = System::Drawing::Point(281, 4);
			this->lbPreviousCount->Name = L"lbPreviousCount";
			this->lbPreviousCount->Size = System::Drawing::Size(19, 13);
			this->lbPreviousCount->TabIndex = 16;
			this->lbPreviousCount->Text = L"----";
			// 
			// lbPreviousType
			// 
			this->lbPreviousType->AutoSize = true;
			this->lbPreviousType->Location = System::Drawing::Point(281, 25);
			this->lbPreviousType->Name = L"lbPreviousType";
			this->lbPreviousType->Size = System::Drawing::Size(19, 13);
			this->lbPreviousType->TabIndex = 15;
			this->lbPreviousType->Text = L"----";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(213, 88);
			this->label24->Margin = System::Windows::Forms::Padding(4);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(61, 13);
			this->label24->TabIndex = 14;
			this->label24->Text = L"Ball Speed:";
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(195, 67);
			this->label25->Margin = System::Windows::Forms::Padding(4);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(79, 13);
			this->label25->TabIndex = 13;
			this->label25->Text = L"Racquet Zone:";
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(209, 25);
			this->label27->Margin = System::Windows::Forms::Padding(4);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(65, 13);
			this->label27->TabIndex = 12;
			this->label27->Text = L"Stoke Type:";
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(210, 46);
			this->label28->Margin = System::Windows::Forms::Padding(4);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(64, 13);
			this->label28->TabIndex = 11;
			this->label28->Text = L"Event Time:";
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(171, 4);
			this->label29->Margin = System::Windows::Forms::Padding(4);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(103, 13);
			this->label29->TabIndex = 10;
			this->label29->Text = L"Stroke Event Count:";
			// 
			// tabPage7
			// 
			this->tabPage7->Controls->Add(this->tabControl3);
			this->tabPage7->Location = System::Drawing::Point(4, 22);
			this->tabPage7->Name = L"tabPage7";
			this->tabPage7->Size = System::Drawing::Size(392, 114);
			this->tabPage7->TabIndex = 4;
			this->tabPage7->Text = L"Scoring";
			this->tabPage7->UseVisualStyleBackColor = true;
			// 
			// tabControl3
			// 
			this->tabControl3->Controls->Add(this->tabPage8);
			this->tabControl3->Controls->Add(this->tabPage9);
			this->tabControl3->Controls->Add(this->tabPage10);
			this->tabControl3->Controls->Add(this->tabPage11);
			this->tabControl3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl3->Location = System::Drawing::Point(0, 0);
			this->tabControl3->Name = L"tabControl3";
			this->tabControl3->SelectedIndex = 0;
			this->tabControl3->Size = System::Drawing::Size(392, 114);
			this->tabControl3->TabIndex = 0;
			// 
			// tabPage8
			// 
			this->tabPage8->Controls->Add(this->groupBox5);
			this->tabPage8->Controls->Add(this->label35);
			this->tabPage8->Controls->Add(this->label34);
			this->tabPage8->Controls->Add(this->nmOpponentGameScore);
			this->tabPage8->Controls->Add(this->nmMyGameScore);
			this->tabPage8->Controls->Add(this->cbInvalidMyGameScore);
			this->tabPage8->Controls->Add(this->cbInvalidOpponentGameScore);
			this->tabPage8->Controls->Add(this->btSendGameScore);
			this->tabPage8->Controls->Add(this->btMarkStroke);
			this->tabPage8->Controls->Add(this->groupBox1);
			this->tabPage8->Location = System::Drawing::Point(4, 22);
			this->tabPage8->Name = L"tabPage8";
			this->tabPage8->Padding = System::Windows::Forms::Padding(3);
			this->tabPage8->Size = System::Drawing::Size(384, 88);
			this->tabPage8->TabIndex = 0;
			this->tabPage8->Text = L"Event Data";
			this->tabPage8->UseVisualStyleBackColor = true;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->btTimerStop);
			this->groupBox5->Controls->Add(this->btTimerStart);
			this->groupBox5->Location = System::Drawing::Point(79, 3);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(70, 77);
			this->groupBox5->TabIndex = 7;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Timer";
			// 
			// btTimerStop
			// 
			this->btTimerStop->Location = System::Drawing::Point(6, 48);
			this->btTimerStop->Name = L"btTimerStop";
			this->btTimerStop->Size = System::Drawing::Size(58, 23);
			this->btTimerStop->TabIndex = 1;
			this->btTimerStop->Text = L"Stop";
			this->btTimerStop->UseVisualStyleBackColor = true;
			this->btTimerStop->Click += gcnew System::EventHandler(this, &RacquetDisplay::btTimerStop_Click);
			// 
			// btTimerStart
			// 
			this->btTimerStart->Location = System::Drawing::Point(6, 19);
			this->btTimerStart->Name = L"btTimerStart";
			this->btTimerStart->Size = System::Drawing::Size(58, 23);
			this->btTimerStart->TabIndex = 0;
			this->btTimerStart->Text = L"Start";
			this->btTimerStart->UseVisualStyleBackColor = true;
			this->btTimerStart->Click += gcnew System::EventHandler(this, &RacquetDisplay::btTimerStart_Click);
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(195, 8);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(56, 13);
			this->label35->TabIndex = 6;
			this->label35->Text = L"My Points:";
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(155, 33);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(96, 13);
			this->label34->TabIndex = 5;
			this->label34->Text = L"Opponent\'s Points:";
			// 
			// nmOpponentGameScore
			// 
			this->nmOpponentGameScore->Location = System::Drawing::Point(256, 31);
			this->nmOpponentGameScore->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmOpponentGameScore->Name = L"nmOpponentGameScore";
			this->nmOpponentGameScore->Size = System::Drawing::Size(65, 20);
			this->nmOpponentGameScore->TabIndex = 4;
			// 
			// nmMyGameScore
			// 
			this->nmMyGameScore->Location = System::Drawing::Point(256, 6);
			this->nmMyGameScore->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmMyGameScore->Name = L"nmMyGameScore";
			this->nmMyGameScore->Size = System::Drawing::Size(65, 20);
			this->nmMyGameScore->TabIndex = 3;
			// 
			// cbInvalidMyGameScore
			// 
			this->cbInvalidMyGameScore->AutoSize = true;
			this->cbInvalidMyGameScore->Location = System::Drawing::Point(326, 6);
			this->cbInvalidMyGameScore->Name = L"cbInvalidMyGameScore";
			this->cbInvalidMyGameScore->Size = System::Drawing::Size(57, 17);
			this->cbInvalidMyGameScore->TabIndex = 5;
			this->cbInvalidMyGameScore->Text = L"Invalid";
			this->cbInvalidMyGameScore->UseVisualStyleBackColor = true;
			this->cbInvalidMyGameScore->CheckedChanged += gcnew System::EventHandler(this, &RacquetDisplay::cbInvalidMyGameScore_CheckedChanged);
			// 
			// cbInvalidOpponentGameScore
			// 
			this->cbInvalidOpponentGameScore->AutoSize = true;
			this->cbInvalidOpponentGameScore->Location = System::Drawing::Point(326, 31);
			this->cbInvalidOpponentGameScore->Name = L"cbInvalidOpponentGameScore";
			this->cbInvalidOpponentGameScore->Size = System::Drawing::Size(57, 17);
			this->cbInvalidOpponentGameScore->TabIndex = 6;
			this->cbInvalidOpponentGameScore->Text = L"Invalid";
			this->cbInvalidOpponentGameScore->UseVisualStyleBackColor = true;
			this->cbInvalidOpponentGameScore->CheckedChanged += gcnew System::EventHandler(this, &RacquetDisplay::cbInvalidOpponentGameScore_CheckedChanged);
			// 
			// btSendGameScore
			// 
			this->btSendGameScore->Location = System::Drawing::Point(256, 60);
			this->btSendGameScore->Name = L"btSendGameScore";
			this->btSendGameScore->Size = System::Drawing::Size(119, 23);
			this->btSendGameScore->TabIndex = 2;
			this->btSendGameScore->Text = L"Send Game Score";
			this->btSendGameScore->UseVisualStyleBackColor = true;
			this->btSendGameScore->Click += gcnew System::EventHandler(this, &RacquetDisplay::btSendGameScore_Click);
			// 
			// btMarkStroke
			// 
			this->btMarkStroke->Location = System::Drawing::Point(175, 60);
			this->btMarkStroke->Name = L"btMarkStroke";
			this->btMarkStroke->Size = System::Drawing::Size(75, 23);
			this->btMarkStroke->TabIndex = 1;
			this->btMarkStroke->Text = L"Mark Stroke";
			this->btMarkStroke->UseVisualStyleBackColor = true;
			this->btMarkStroke->Click += gcnew System::EventHandler(this, &RacquetDisplay::btMarkStroke_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->btSessionStop);
			this->groupBox1->Controls->Add(this->btSessionStart);
			this->groupBox1->Location = System::Drawing::Point(3, 3);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(70, 77);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Session";
			// 
			// btSessionStop
			// 
			this->btSessionStop->Location = System::Drawing::Point(6, 48);
			this->btSessionStop->Name = L"btSessionStop";
			this->btSessionStop->Size = System::Drawing::Size(58, 23);
			this->btSessionStop->TabIndex = 1;
			this->btSessionStop->Text = L"Stop";
			this->btSessionStop->UseVisualStyleBackColor = true;
			this->btSessionStop->Click += gcnew System::EventHandler(this, &RacquetDisplay::btSessionStop_Click);
			// 
			// btSessionStart
			// 
			this->btSessionStart->Location = System::Drawing::Point(6, 19);
			this->btSessionStart->Name = L"btSessionStart";
			this->btSessionStart->Size = System::Drawing::Size(58, 23);
			this->btSessionStart->TabIndex = 0;
			this->btSessionStart->Text = L"Start";
			this->btSessionStart->UseVisualStyleBackColor = true;
			this->btSessionStart->Click += gcnew System::EventHandler(this, &RacquetDisplay::btSessionStart_Click);
			// 
			// tabPage9
			// 
			this->tabPage9->Controls->Add(this->btSendSessionData);
			this->tabPage9->Controls->Add(this->label38);
			this->tabPage9->Controls->Add(this->cbGameMode);
			this->tabPage9->Controls->Add(this->groupBox2);
			this->tabPage9->Location = System::Drawing::Point(4, 22);
			this->tabPage9->Name = L"tabPage9";
			this->tabPage9->Padding = System::Windows::Forms::Padding(3);
			this->tabPage9->Size = System::Drawing::Size(384, 88);
			this->tabPage9->TabIndex = 1;
			this->tabPage9->Text = L"Session Data";
			this->tabPage9->UseVisualStyleBackColor = true;
			// 
			// btSendSessionData
			// 
			this->btSendSessionData->Location = System::Drawing::Point(229, 57);
			this->btSendSessionData->Name = L"btSendSessionData";
			this->btSendSessionData->Size = System::Drawing::Size(143, 23);
			this->btSendSessionData->TabIndex = 5;
			this->btSendSessionData->Text = L"Send Session Data";
			this->btSendSessionData->UseVisualStyleBackColor = true;
			this->btSendSessionData->Click += gcnew System::EventHandler(this, &RacquetDisplay::btSendSessionData_Click);
			// 
			// label38
			// 
			this->label38->AutoSize = true;
			this->label38->Location = System::Drawing::Point(226, 22);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(68, 13);
			this->label38->TabIndex = 4;
			this->label38->Text = L"Game Mode:";
			// 
			// cbGameMode
			// 
			this->cbGameMode->FormattingEnabled = true;
			this->cbGameMode->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"Warm Up", L"Match", L"Exercise", L"Challenge"});
			this->cbGameMode->Location = System::Drawing::Point(300, 19);
			this->cbGameMode->Name = L"cbGameMode";
			this->cbGameMode->Size = System::Drawing::Size(72, 21);
			this->cbGameMode->TabIndex = 3;
			this->cbGameMode->Text = L"Match";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->cbInvalidOpponentSetsWon);
			this->groupBox2->Controls->Add(this->cbInvalidMySetsWon);
			this->groupBox2->Controls->Add(this->label37);
			this->groupBox2->Controls->Add(this->label36);
			this->groupBox2->Controls->Add(this->nmOpponentSetsWon);
			this->groupBox2->Controls->Add(this->nmMySetsWon);
			this->groupBox2->Location = System::Drawing::Point(6, 6);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(198, 74);
			this->groupBox2->TabIndex = 2;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"# Sets Won";
			// 
			// cbInvalidOpponentSetsWon
			// 
			this->cbInvalidOpponentSetsWon->AutoSize = true;
			this->cbInvalidOpponentSetsWon->Location = System::Drawing::Point(138, 50);
			this->cbInvalidOpponentSetsWon->Name = L"cbInvalidOpponentSetsWon";
			this->cbInvalidOpponentSetsWon->Size = System::Drawing::Size(57, 17);
			this->cbInvalidOpponentSetsWon->TabIndex = 4;
			this->cbInvalidOpponentSetsWon->Text = L"Invalid";
			this->cbInvalidOpponentSetsWon->UseVisualStyleBackColor = true;
			this->cbInvalidOpponentSetsWon->CheckedChanged += gcnew System::EventHandler(this, &RacquetDisplay::cbInvalidOpponentSetsWon_CheckedChanged);
			// 
			// cbInvalidMySetsWon
			// 
			this->cbInvalidMySetsWon->AutoSize = true;
			this->cbInvalidMySetsWon->Location = System::Drawing::Point(138, 22);
			this->cbInvalidMySetsWon->Name = L"cbInvalidMySetsWon";
			this->cbInvalidMySetsWon->Size = System::Drawing::Size(57, 17);
			this->cbInvalidMySetsWon->TabIndex = 3;
			this->cbInvalidMySetsWon->Text = L"Invalid";
			this->cbInvalidMySetsWon->UseVisualStyleBackColor = true;
			this->cbInvalidMySetsWon->CheckedChanged += gcnew System::EventHandler(this, &RacquetDisplay::cbInvalidMySetsWon_CheckedChanged);
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Location = System::Drawing::Point(6, 50);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(54, 13);
			this->label37->TabIndex = 2;
			this->label37->Text = L"Opponent";
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Location = System::Drawing::Point(38, 21);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(22, 13);
			this->label36->TabIndex = 2;
			this->label36->Text = L"Me";
			// 
			// nmOpponentSetsWon
			// 
			this->nmOpponentSetsWon->Location = System::Drawing::Point(66, 48);
			this->nmOpponentSetsWon->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmOpponentSetsWon->Name = L"nmOpponentSetsWon";
			this->nmOpponentSetsWon->Size = System::Drawing::Size(66, 20);
			this->nmOpponentSetsWon->TabIndex = 1;
			this->nmOpponentSetsWon->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// nmMySetsWon
			// 
			this->nmMySetsWon->Location = System::Drawing::Point(66, 19);
			this->nmMySetsWon->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmMySetsWon->Name = L"nmMySetsWon";
			this->nmMySetsWon->Size = System::Drawing::Size(66, 20);
			this->nmMySetsWon->TabIndex = 0;
			this->nmMySetsWon->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			// 
			// tabPage10
			// 
			this->tabPage10->Controls->Add(this->cbInvalidOpponentSetCount);
			this->tabPage10->Controls->Add(this->cbInvalidMySetCount);
			this->tabPage10->Controls->Add(this->label41);
			this->tabPage10->Controls->Add(this->label40);
			this->tabPage10->Controls->Add(this->label39);
			this->tabPage10->Controls->Add(this->btSendSetCount);
			this->tabPage10->Controls->Add(this->nmMySetCount);
			this->tabPage10->Controls->Add(this->nmOpponentSetCount);
			this->tabPage10->Location = System::Drawing::Point(4, 22);
			this->tabPage10->Name = L"tabPage10";
			this->tabPage10->Size = System::Drawing::Size(384, 88);
			this->tabPage10->TabIndex = 2;
			this->tabPage10->Text = L"Lap Data";
			this->tabPage10->UseVisualStyleBackColor = true;
			// 
			// cbInvalidOpponentSetCount
			// 
			this->cbInvalidOpponentSetCount->AutoSize = true;
			this->cbInvalidOpponentSetCount->Location = System::Drawing::Point(142, 57);
			this->cbInvalidOpponentSetCount->Name = L"cbInvalidOpponentSetCount";
			this->cbInvalidOpponentSetCount->Size = System::Drawing::Size(57, 17);
			this->cbInvalidOpponentSetCount->TabIndex = 7;
			this->cbInvalidOpponentSetCount->Text = L"Invalid";
			this->cbInvalidOpponentSetCount->UseVisualStyleBackColor = true;
			this->cbInvalidOpponentSetCount->CheckedChanged += gcnew System::EventHandler(this, &RacquetDisplay::cbInvalidOpponentSetCount_CheckedChanged);
			// 
			// cbInvalidMySetCount
			// 
			this->cbInvalidMySetCount->AutoSize = true;
			this->cbInvalidMySetCount->Location = System::Drawing::Point(142, 31);
			this->cbInvalidMySetCount->Name = L"cbInvalidMySetCount";
			this->cbInvalidMySetCount->Size = System::Drawing::Size(57, 17);
			this->cbInvalidMySetCount->TabIndex = 6;
			this->cbInvalidMySetCount->Text = L"Invalid";
			this->cbInvalidMySetCount->UseVisualStyleBackColor = true;
			this->cbInvalidMySetCount->CheckedChanged += gcnew System::EventHandler(this, &RacquetDisplay::cbInvalidMySetCount_CheckedChanged);
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Location = System::Drawing::Point(53, 32);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(22, 13);
			this->label41->TabIndex = 5;
			this->label41->Text = L"Me";
			// 
			// label40
			// 
			this->label40->AutoSize = true;
			this->label40->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label40->Location = System::Drawing::Point(86, 5);
			this->label40->Margin = System::Windows::Forms::Padding(3, 5, 3, 3);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(54, 13);
			this->label40->TabIndex = 4;
			this->label40->Text = L"Set Count";
			// 
			// label39
			// 
			this->label39->AutoSize = true;
			this->label39->Location = System::Drawing::Point(21, 58);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(54, 13);
			this->label39->TabIndex = 2;
			this->label39->Text = L"Opponent";
			// 
			// btSendSetCount
			// 
			this->btSendSetCount->Location = System::Drawing::Point(243, 57);
			this->btSendSetCount->Name = L"btSendSetCount";
			this->btSendSetCount->Size = System::Drawing::Size(132, 23);
			this->btSendSetCount->TabIndex = 3;
			this->btSendSetCount->Text = L"Send Set Count";
			this->btSendSetCount->UseVisualStyleBackColor = true;
			this->btSendSetCount->Click += gcnew System::EventHandler(this, &RacquetDisplay::btSendSetCount_Click);
			// 
			// nmMySetCount
			// 
			this->nmMySetCount->Location = System::Drawing::Point(81, 30);
			this->nmMySetCount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmMySetCount->Name = L"nmMySetCount";
			this->nmMySetCount->Size = System::Drawing::Size(55, 20);
			this->nmMySetCount->TabIndex = 0;
			// 
			// nmOpponentSetCount
			// 
			this->nmOpponentSetCount->Location = System::Drawing::Point(81, 56);
			this->nmOpponentSetCount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmOpponentSetCount->Name = L"nmOpponentSetCount";
			this->nmOpponentSetCount->Size = System::Drawing::Size(55, 20);
			this->nmOpponentSetCount->TabIndex = 1;
			// 
			// tabPage11
			// 
			this->tabPage11->Controls->Add(this->cbInvalidOpponentGameCount);
			this->tabPage11->Controls->Add(this->cbInvalidMyGameCount);
			this->tabPage11->Controls->Add(this->label42);
			this->tabPage11->Controls->Add(this->label43);
			this->tabPage11->Controls->Add(this->label44);
			this->tabPage11->Controls->Add(this->btSendGameCount);
			this->tabPage11->Controls->Add(this->nmMyGameCount);
			this->tabPage11->Controls->Add(this->nmOpponentGameCount);
			this->tabPage11->Location = System::Drawing::Point(4, 22);
			this->tabPage11->Name = L"tabPage11";
			this->tabPage11->Size = System::Drawing::Size(384, 88);
			this->tabPage11->TabIndex = 3;
			this->tabPage11->Text = L"Length Data";
			this->tabPage11->UseVisualStyleBackColor = true;
			// 
			// cbInvalidOpponentGameCount
			// 
			this->cbInvalidOpponentGameCount->AutoSize = true;
			this->cbInvalidOpponentGameCount->Location = System::Drawing::Point(142, 57);
			this->cbInvalidOpponentGameCount->Name = L"cbInvalidOpponentGameCount";
			this->cbInvalidOpponentGameCount->Size = System::Drawing::Size(57, 17);
			this->cbInvalidOpponentGameCount->TabIndex = 15;
			this->cbInvalidOpponentGameCount->Text = L"Invalid";
			this->cbInvalidOpponentGameCount->UseVisualStyleBackColor = true;
			this->cbInvalidOpponentGameCount->CheckedChanged += gcnew System::EventHandler(this, &RacquetDisplay::cbInvalidOpponentGameCount_CheckedChanged);
			// 
			// cbInvalidMyGameCount
			// 
			this->cbInvalidMyGameCount->AutoSize = true;
			this->cbInvalidMyGameCount->Location = System::Drawing::Point(142, 31);
			this->cbInvalidMyGameCount->Name = L"cbInvalidMyGameCount";
			this->cbInvalidMyGameCount->Size = System::Drawing::Size(57, 17);
			this->cbInvalidMyGameCount->TabIndex = 14;
			this->cbInvalidMyGameCount->Text = L"Invalid";
			this->cbInvalidMyGameCount->UseVisualStyleBackColor = true;
			this->cbInvalidMyGameCount->CheckedChanged += gcnew System::EventHandler(this, &RacquetDisplay::cbInvalidMyGameCount_CheckedChanged);
			// 
			// label42
			// 
			this->label42->AutoSize = true;
			this->label42->Location = System::Drawing::Point(53, 32);
			this->label42->Name = L"label42";
			this->label42->Size = System::Drawing::Size(22, 13);
			this->label42->TabIndex = 13;
			this->label42->Text = L"Me";
			// 
			// label43
			// 
			this->label43->AutoSize = true;
			this->label43->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label43->Location = System::Drawing::Point(86, 5);
			this->label43->Margin = System::Windows::Forms::Padding(3, 5, 3, 3);
			this->label43->Name = L"label43";
			this->label43->Size = System::Drawing::Size(66, 13);
			this->label43->TabIndex = 12;
			this->label43->Text = L"Game Count";
			// 
			// label44
			// 
			this->label44->AutoSize = true;
			this->label44->Location = System::Drawing::Point(21, 58);
			this->label44->Name = L"label44";
			this->label44->Size = System::Drawing::Size(54, 13);
			this->label44->TabIndex = 10;
			this->label44->Text = L"Opponent";
			// 
			// btSendGameCount
			// 
			this->btSendGameCount->Location = System::Drawing::Point(243, 57);
			this->btSendGameCount->Name = L"btSendGameCount";
			this->btSendGameCount->Size = System::Drawing::Size(132, 23);
			this->btSendGameCount->TabIndex = 11;
			this->btSendGameCount->Text = L"Send Game Count";
			this->btSendGameCount->UseVisualStyleBackColor = true;
			this->btSendGameCount->Click += gcnew System::EventHandler(this, &RacquetDisplay::btSendGameCount_Click);
			// 
			// nmMyGameCount
			// 
			this->nmMyGameCount->Location = System::Drawing::Point(81, 30);
			this->nmMyGameCount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmMyGameCount->Name = L"nmMyGameCount";
			this->nmMyGameCount->Size = System::Drawing::Size(55, 20);
			this->nmMyGameCount->TabIndex = 8;
			// 
			// nmOpponentGameCount
			// 
			this->nmOpponentGameCount->Location = System::Drawing::Point(81, 56);
			this->nmOpponentGameCount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmOpponentGameCount->Name = L"nmOpponentGameCount";
			this->nmOpponentGameCount->Size = System::Drawing::Size(55, 20);
			this->nmOpponentGameCount->TabIndex = 9;
			// 
			// tabPage17
			// 
			this->tabPage17->Controls->Add(this->tabControl5);
			this->tabPage17->Location = System::Drawing::Point(4, 22);
			this->tabPage17->Name = L"tabPage17";
			this->tabPage17->Size = System::Drawing::Size(392, 114);
			this->tabPage17->TabIndex = 5;
			this->tabPage17->Text = L"Stats";
			this->tabPage17->UseVisualStyleBackColor = true;
			// 
			// tabControl5
			// 
			this->tabControl5->Controls->Add(this->tabPage18);
			this->tabControl5->Controls->Add(this->tabPage19);
			this->tabControl5->Controls->Add(this->tabPage20);
			this->tabControl5->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl5->Location = System::Drawing::Point(0, 0);
			this->tabControl5->Name = L"tabControl5";
			this->tabControl5->SelectedIndex = 0;
			this->tabControl5->Size = System::Drawing::Size(392, 114);
			this->tabControl5->TabIndex = 0;
			// 
			// tabPage18
			// 
			this->tabPage18->Controls->Add(this->btSendMetabolicData);
			this->tabPage18->Controls->Add(this->groupBox_Calories);
			this->tabPage18->Controls->Add(this->groupBox_HR);
			this->tabPage18->Location = System::Drawing::Point(4, 22);
			this->tabPage18->Name = L"tabPage18";
			this->tabPage18->Padding = System::Windows::Forms::Padding(3);
			this->tabPage18->Size = System::Drawing::Size(384, 88);
			this->tabPage18->TabIndex = 0;
			this->tabPage18->Text = L"Fitness Data";
			this->tabPage18->UseVisualStyleBackColor = true;
			// 
			// btSendMetabolicData
			// 
			this->btSendMetabolicData->Location = System::Drawing::Point(178, 62);
			this->btSendMetabolicData->Name = L"btSendMetabolicData";
			this->btSendMetabolicData->Size = System::Drawing::Size(175, 23);
			this->btSendMetabolicData->TabIndex = 5;
			this->btSendMetabolicData->Text = L"Send Metabolic Data";
			this->btSendMetabolicData->UseVisualStyleBackColor = true;
			this->btSendMetabolicData->Click += gcnew System::EventHandler(this, &RacquetDisplay::btSendMetabolicData_Click);
			// 
			// groupBox_Calories
			// 
			this->groupBox_Calories->Controls->Add(this->label52);
			this->groupBox_Calories->Controls->Add(this->nmTotalCalories);
			this->groupBox_Calories->Controls->Add(this->label51);
			this->groupBox_Calories->Location = System::Drawing::Point(150, 3);
			this->groupBox_Calories->Name = L"groupBox_Calories";
			this->groupBox_Calories->Size = System::Drawing::Size(231, 53);
			this->groupBox_Calories->TabIndex = 4;
			this->groupBox_Calories->TabStop = false;
			this->groupBox_Calories->Text = L"Calories";
			// 
			// label52
			// 
			this->label52->AutoSize = true;
			this->label52->Location = System::Drawing::Point(176, 20);
			this->label52->Name = L"label52";
			this->label52->Size = System::Drawing::Size(27, 13);
			this->label52->TabIndex = 2;
			this->label52->Text = L"kcal";
			// 
			// nmTotalCalories
			// 
			this->nmTotalCalories->Location = System::Drawing::Point(65, 18);
			this->nmTotalCalories->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65535, 0, 0, 0});
			this->nmTotalCalories->Name = L"nmTotalCalories";
			this->nmTotalCalories->Size = System::Drawing::Size(105, 20);
			this->nmTotalCalories->TabIndex = 1;
			// 
			// label51
			// 
			this->label51->AutoSize = true;
			this->label51->Location = System::Drawing::Point(25, 20);
			this->label51->Name = L"label51";
			this->label51->Size = System::Drawing::Size(34, 13);
			this->label51->TabIndex = 0;
			this->label51->Text = L"Total:";
			// 
			// groupBox_HR
			// 
			this->groupBox_HR->Controls->Add(this->label54);
			this->groupBox_HR->Controls->Add(this->label53);
			this->groupBox_HR->Controls->Add(this->nmAvgHR);
			this->groupBox_HR->Controls->Add(this->nmMaxHR);
			this->groupBox_HR->Controls->Add(this->label50);
			this->groupBox_HR->Controls->Add(this->label49);
			this->groupBox_HR->Location = System::Drawing::Point(3, 3);
			this->groupBox_HR->Name = L"groupBox_HR";
			this->groupBox_HR->Size = System::Drawing::Size(141, 82);
			this->groupBox_HR->TabIndex = 3;
			this->groupBox_HR->TabStop = false;
			this->groupBox_HR->Text = L"Heart Rate";
			// 
			// label54
			// 
			this->label54->AutoSize = true;
			this->label54->Location = System::Drawing::Point(110, 53);
			this->label54->Name = L"label54";
			this->label54->Size = System::Drawing::Size(27, 13);
			this->label54->TabIndex = 5;
			this->label54->Text = L"bpm";
			// 
			// label53
			// 
			this->label53->AutoSize = true;
			this->label53->Location = System::Drawing::Point(110, 20);
			this->label53->Name = L"label53";
			this->label53->Size = System::Drawing::Size(27, 13);
			this->label53->TabIndex = 4;
			this->label53->Text = L"bpm";
			// 
			// nmAvgHR
			// 
			this->nmAvgHR->Location = System::Drawing::Point(63, 51);
			this->nmAvgHR->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->nmAvgHR->Name = L"nmAvgHR";
			this->nmAvgHR->Size = System::Drawing::Size(41, 20);
			this->nmAvgHR->TabIndex = 3;
			this->nmAvgHR->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {165, 0, 0, 0});
			// 
			// nmMaxHR
			// 
			this->nmMaxHR->Location = System::Drawing::Point(63, 18);
			this->nmMaxHR->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->nmMaxHR->Name = L"nmMaxHR";
			this->nmMaxHR->Size = System::Drawing::Size(41, 20);
			this->nmMaxHR->TabIndex = 2;
			this->nmMaxHR->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {184, 0, 0, 0});
			// 
			// label50
			// 
			this->label50->AutoSize = true;
			this->label50->Location = System::Drawing::Point(7, 53);
			this->label50->Name = L"label50";
			this->label50->Size = System::Drawing::Size(50, 13);
			this->label50->TabIndex = 1;
			this->label50->Text = L"Average:";
			// 
			// label49
			// 
			this->label49->AutoSize = true;
			this->label49->Location = System::Drawing::Point(3, 20);
			this->label49->Name = L"label49";
			this->label49->Size = System::Drawing::Size(54, 13);
			this->label49->TabIndex = 0;
			this->label49->Text = L"Maximum:";
			// 
			// tabPage19
			// 
			this->tabPage19->Controls->Add(this->label59);
			this->tabPage19->Controls->Add(this->label58);
			this->tabPage19->Controls->Add(this->label55);
			this->tabPage19->Controls->Add(this->label56);
			this->tabPage19->Controls->Add(this->label57);
			this->tabPage19->Controls->Add(this->btSendSpeedData);
			this->tabPage19->Controls->Add(this->nmMaxRunSpeed);
			this->tabPage19->Controls->Add(this->nmAvgRunSpeed);
			this->tabPage19->Location = System::Drawing::Point(4, 22);
			this->tabPage19->Name = L"tabPage19";
			this->tabPage19->Padding = System::Windows::Forms::Padding(3);
			this->tabPage19->Size = System::Drawing::Size(384, 88);
			this->tabPage19->TabIndex = 1;
			this->tabPage19->Text = L"Speed Data";
			this->tabPage19->UseVisualStyleBackColor = true;
			// 
			// label59
			// 
			this->label59->AutoSize = true;
			this->label59->Location = System::Drawing::Point(146, 63);
			this->label59->Name = L"label59";
			this->label59->Size = System::Drawing::Size(33, 13);
			this->label59->TabIndex = 23;
			this->label59->Text = L"mm/s";
			// 
			// label58
			// 
			this->label58->AutoSize = true;
			this->label58->Location = System::Drawing::Point(146, 37);
			this->label58->Name = L"label58";
			this->label58->Size = System::Drawing::Size(33, 13);
			this->label58->TabIndex = 22;
			this->label58->Text = L"mm/s";
			// 
			// label55
			// 
			this->label55->AutoSize = true;
			this->label55->Location = System::Drawing::Point(28, 37);
			this->label55->Name = L"label55";
			this->label55->Size = System::Drawing::Size(51, 13);
			this->label55->TabIndex = 21;
			this->label55->Text = L"Maximum";
			// 
			// label56
			// 
			this->label56->AutoSize = true;
			this->label56->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label56->Location = System::Drawing::Point(59, 7);
			this->label56->Margin = System::Windows::Forms::Padding(3, 5, 3, 3);
			this->label56->Name = L"label56";
			this->label56->Size = System::Drawing::Size(81, 13);
			this->label56->TabIndex = 20;
			this->label56->Text = L"Running Speed";
			// 
			// label57
			// 
			this->label57->AutoSize = true;
			this->label57->Location = System::Drawing::Point(32, 63);
			this->label57->Name = L"label57";
			this->label57->Size = System::Drawing::Size(47, 13);
			this->label57->TabIndex = 18;
			this->label57->Text = L"Average";
			// 
			// btSendSpeedData
			// 
			this->btSendSpeedData->Location = System::Drawing::Point(224, 58);
			this->btSendSpeedData->Name = L"btSendSpeedData";
			this->btSendSpeedData->Size = System::Drawing::Size(132, 23);
			this->btSendSpeedData->TabIndex = 19;
			this->btSendSpeedData->Text = L"Send Speed Data";
			this->btSendSpeedData->UseVisualStyleBackColor = true;
			this->btSendSpeedData->Click += gcnew System::EventHandler(this, &RacquetDisplay::btSendSpeedData_Click);
			// 
			// nmMaxRunSpeed
			// 
			this->nmMaxRunSpeed->Location = System::Drawing::Point(85, 33);
			this->nmMaxRunSpeed->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmMaxRunSpeed->Name = L"nmMaxRunSpeed";
			this->nmMaxRunSpeed->Size = System::Drawing::Size(55, 20);
			this->nmMaxRunSpeed->TabIndex = 16;
			// 
			// nmAvgRunSpeed
			// 
			this->nmAvgRunSpeed->Location = System::Drawing::Point(85, 59);
			this->nmAvgRunSpeed->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 0});
			this->nmAvgRunSpeed->Name = L"nmAvgRunSpeed";
			this->nmAvgRunSpeed->Size = System::Drawing::Size(55, 20);
			this->nmAvgRunSpeed->TabIndex = 17;
			// 
			// tabPage20
			// 
			this->tabPage20->Controls->Add(this->label60);
			this->tabPage20->Controls->Add(this->label61);
			this->tabPage20->Controls->Add(this->btSendDistanceData);
			this->tabPage20->Controls->Add(this->nmTotalDistance);
			this->tabPage20->Location = System::Drawing::Point(4, 22);
			this->tabPage20->Name = L"tabPage20";
			this->tabPage20->Size = System::Drawing::Size(384, 88);
			this->tabPage20->TabIndex = 2;
			this->tabPage20->Text = L"Distance Data";
			this->tabPage20->UseVisualStyleBackColor = true;
			// 
			// label60
			// 
			this->label60->AutoSize = true;
			this->label60->Location = System::Drawing::Point(191, 38);
			this->label60->Name = L"label60";
			this->label60->Size = System::Drawing::Size(21, 13);
			this->label60->TabIndex = 23;
			this->label60->Text = L"cm";
			// 
			// label61
			// 
			this->label61->AutoSize = true;
			this->label61->Location = System::Drawing::Point(15, 38);
			this->label61->Name = L"label61";
			this->label61->Size = System::Drawing::Size(79, 13);
			this->label61->TabIndex = 21;
			this->label61->Text = L"Total Distance:";
			// 
			// btSendDistanceData
			// 
			this->btSendDistanceData->Location = System::Drawing::Point(237, 33);
			this->btSendDistanceData->Name = L"btSendDistanceData";
			this->btSendDistanceData->Size = System::Drawing::Size(132, 23);
			this->btSendDistanceData->TabIndex = 22;
			this->btSendDistanceData->Text = L"Send Distance Data";
			this->btSendDistanceData->UseVisualStyleBackColor = true;
			this->btSendDistanceData->Click += gcnew System::EventHandler(this, &RacquetDisplay::btSendDistanceData_Click);
			// 
			// nmTotalDistance
			// 
			this->nmTotalDistance->Location = System::Drawing::Point(100, 34);
			this->nmTotalDistance->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {-1, 0, 0, 0});
			this->nmTotalDistance->Name = L"nmTotalDistance";
			this->nmTotalDistance->Size = System::Drawing::Size(85, 20);
			this->nmTotalDistance->TabIndex = 20;
			// 
			// tabPage6
			// 
			this->tabPage6->Controls->Add(this->tabControl4);
			this->tabPage6->Location = System::Drawing::Point(4, 22);
			this->tabPage6->Name = L"tabPage6";
			this->tabPage6->Size = System::Drawing::Size(392, 114);
			this->tabPage6->TabIndex = 3;
			this->tabPage6->Text = L"Common Data";
			this->tabPage6->UseVisualStyleBackColor = true;
			// 
			// tabControl4
			// 
			this->tabControl4->Controls->Add(this->tabPage12);
			this->tabControl4->Controls->Add(this->tabPage13);
			this->tabControl4->Location = System::Drawing::Point(0, 0);
			this->tabControl4->Name = L"tabControl4";
			this->tabControl4->SelectedIndex = 0;
			this->tabControl4->Size = System::Drawing::Size(386, 112);
			this->tabControl4->TabIndex = 0;
			// 
			// tabPage12
			// 
			this->tabPage12->Controls->Add(this->label_Glb_SoftwareVer);
			this->tabPage12->Controls->Add(this->label_Glb_SerialNum);
			this->tabPage12->Controls->Add(this->lbSerialNumber);
			this->tabPage12->Controls->Add(this->lbSoftwareVer);
			this->tabPage12->Controls->Add(this->label_Glb_HardwareVer);
			this->tabPage12->Controls->Add(this->label_Glb_ManfID);
			this->tabPage12->Controls->Add(this->lbModelNumber);
			this->tabPage12->Controls->Add(this->lbHardwareVer);
			this->tabPage12->Controls->Add(this->lbManfId);
			this->tabPage12->Controls->Add(this->label_Glb_ModelNum);
			this->tabPage12->Location = System::Drawing::Point(4, 22);
			this->tabPage12->Name = L"tabPage12";
			this->tabPage12->Padding = System::Windows::Forms::Padding(3);
			this->tabPage12->Size = System::Drawing::Size(378, 86);
			this->tabPage12->TabIndex = 0;
			this->tabPage12->Text = L"Product/Manf. Info";
			this->tabPage12->UseVisualStyleBackColor = true;
			// 
			// label_Glb_SoftwareVer
			// 
			this->label_Glb_SoftwareVer->AutoSize = true;
			this->label_Glb_SoftwareVer->Location = System::Drawing::Point(192, 33);
			this->label_Glb_SoftwareVer->Name = L"label_Glb_SoftwareVer";
			this->label_Glb_SoftwareVer->Size = System::Drawing::Size(71, 13);
			this->label_Glb_SoftwareVer->TabIndex = 20;
			this->label_Glb_SoftwareVer->Text = L"Software Ver:";
			// 
			// label_Glb_SerialNum
			// 
			this->label_Glb_SerialNum->AutoSize = true;
			this->label_Glb_SerialNum->Location = System::Drawing::Point(48, 52);
			this->label_Glb_SerialNum->Name = L"label_Glb_SerialNum";
			this->label_Glb_SerialNum->Size = System::Drawing::Size(46, 13);
			this->label_Glb_SerialNum->TabIndex = 18;
			this->label_Glb_SerialNum->Text = L"Serial #:";
			// 
			// lbSerialNumber
			// 
			this->lbSerialNumber->Location = System::Drawing::Point(100, 52);
			this->lbSerialNumber->Name = L"lbSerialNumber";
			this->lbSerialNumber->Size = System::Drawing::Size(71, 13);
			this->lbSerialNumber->TabIndex = 24;
			this->lbSerialNumber->Text = L"---";
			// 
			// lbSoftwareVer
			// 
			this->lbSoftwareVer->Location = System::Drawing::Point(269, 33);
			this->lbSoftwareVer->Name = L"lbSoftwareVer";
			this->lbSoftwareVer->Size = System::Drawing::Size(27, 13);
			this->lbSoftwareVer->TabIndex = 26;
			this->lbSoftwareVer->Text = L"---";
			// 
			// label_Glb_HardwareVer
			// 
			this->label_Glb_HardwareVer->AutoSize = true;
			this->label_Glb_HardwareVer->Location = System::Drawing::Point(188, 14);
			this->label_Glb_HardwareVer->Name = L"label_Glb_HardwareVer";
			this->label_Glb_HardwareVer->Size = System::Drawing::Size(75, 13);
			this->label_Glb_HardwareVer->TabIndex = 19;
			this->label_Glb_HardwareVer->Text = L"Hardware Ver:";
			// 
			// label_Glb_ManfID
			// 
			this->label_Glb_ManfID->AutoSize = true;
			this->label_Glb_ManfID->Location = System::Drawing::Point(43, 14);
			this->label_Glb_ManfID->Name = L"label_Glb_ManfID";
			this->label_Glb_ManfID->Size = System::Drawing::Size(51, 13);
			this->label_Glb_ManfID->TabIndex = 17;
			this->label_Glb_ManfID->Text = L"Manf. ID:";
			// 
			// lbModelNumber
			// 
			this->lbModelNumber->Location = System::Drawing::Point(100, 33);
			this->lbModelNumber->Name = L"lbModelNumber";
			this->lbModelNumber->Size = System::Drawing::Size(42, 13);
			this->lbModelNumber->TabIndex = 23;
			this->lbModelNumber->Text = L"---";
			// 
			// lbHardwareVer
			// 
			this->lbHardwareVer->Location = System::Drawing::Point(269, 14);
			this->lbHardwareVer->Name = L"lbHardwareVer";
			this->lbHardwareVer->Size = System::Drawing::Size(27, 13);
			this->lbHardwareVer->TabIndex = 25;
			this->lbHardwareVer->Text = L"---";
			// 
			// lbManfId
			// 
			this->lbManfId->Location = System::Drawing::Point(100, 14);
			this->lbManfId->Name = L"lbManfId";
			this->lbManfId->Size = System::Drawing::Size(27, 13);
			this->lbManfId->TabIndex = 22;
			this->lbManfId->Text = L"---";
			// 
			// label_Glb_ModelNum
			// 
			this->label_Glb_ModelNum->AutoSize = true;
			this->label_Glb_ModelNum->Location = System::Drawing::Point(45, 33);
			this->label_Glb_ModelNum->Name = L"label_Glb_ModelNum";
			this->label_Glb_ModelNum->Size = System::Drawing::Size(49, 13);
			this->label_Glb_ModelNum->TabIndex = 21;
			this->label_Glb_ModelNum->Text = L"Model #:";
			// 
			// tabPage13
			// 
			this->tabPage13->Controls->Add(this->label45);
			this->tabPage13->Controls->Add(this->labelTimeRes);
			this->tabPage13->Controls->Add(this->label46);
			this->tabPage13->Controls->Add(this->labelOpTime);
			this->tabPage13->Controls->Add(this->label47);
			this->tabPage13->Controls->Add(this->labelBattVolt);
			this->tabPage13->Controls->Add(this->label48);
			this->tabPage13->Controls->Add(this->labelBattStatus);
			this->tabPage13->Location = System::Drawing::Point(4, 22);
			this->tabPage13->Name = L"tabPage13";
			this->tabPage13->Padding = System::Windows::Forms::Padding(3);
			this->tabPage13->Size = System::Drawing::Size(378, 86);
			this->tabPage13->TabIndex = 1;
			this->tabPage13->Text = L"Battery Status";
			this->tabPage13->UseVisualStyleBackColor = true;
			// 
			// label45
			// 
			this->label45->AutoSize = true;
			this->label45->Location = System::Drawing::Point(32, 22);
			this->label45->Name = L"label45";
			this->label45->Size = System::Drawing::Size(86, 13);
			this->label45->TabIndex = 41;
			this->label45->Text = L"Time Resolution:";
			// 
			// labelTimeRes
			// 
			this->labelTimeRes->AutoSize = true;
			this->labelTimeRes->Location = System::Drawing::Point(124, 22);
			this->labelTimeRes->Name = L"labelTimeRes";
			this->labelTimeRes->Size = System::Drawing::Size(16, 13);
			this->labelTimeRes->TabIndex = 43;
			this->labelTimeRes->Text = L"---";
			// 
			// label46
			// 
			this->label46->AutoSize = true;
			this->label46->Location = System::Drawing::Point(36, 44);
			this->label46->Name = L"label46";
			this->label46->Size = System::Drawing::Size(82, 13);
			this->label46->TabIndex = 38;
			this->label46->Text = L"Operating Time:";
			// 
			// labelOpTime
			// 
			this->labelOpTime->AutoSize = true;
			this->labelOpTime->Location = System::Drawing::Point(124, 44);
			this->labelOpTime->Name = L"labelOpTime";
			this->labelOpTime->Size = System::Drawing::Size(16, 13);
			this->labelOpTime->TabIndex = 42;
			this->labelOpTime->Text = L"---";
			// 
			// label47
			// 
			this->label47->AutoSize = true;
			this->label47->Location = System::Drawing::Point(205, 22);
			this->label47->Name = L"label47";
			this->label47->Size = System::Drawing::Size(82, 13);
			this->label47->TabIndex = 39;
			this->label47->Text = L"Battery Voltage:";
			// 
			// labelBattVolt
			// 
			this->labelBattVolt->AutoSize = true;
			this->labelBattVolt->Location = System::Drawing::Point(293, 22);
			this->labelBattVolt->Name = L"labelBattVolt";
			this->labelBattVolt->Size = System::Drawing::Size(16, 13);
			this->labelBattVolt->TabIndex = 44;
			this->labelBattVolt->Text = L"---";
			// 
			// label48
			// 
			this->label48->AutoSize = true;
			this->label48->Location = System::Drawing::Point(211, 44);
			this->label48->Name = L"label48";
			this->label48->Size = System::Drawing::Size(76, 13);
			this->label48->TabIndex = 40;
			this->label48->Text = L"Battery Status:";
			// 
			// labelBattStatus
			// 
			this->labelBattStatus->AutoSize = true;
			this->labelBattStatus->Location = System::Drawing::Point(293, 44);
			this->labelBattStatus->Name = L"labelBattStatus";
			this->labelBattStatus->Size = System::Drawing::Size(16, 13);
			this->labelBattStatus->TabIndex = 45;
			this->labelBattStatus->Text = L"---";
			// 
			// panel_Display
			// 
			this->panel_Display->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel_Display->Controls->Add(this->lbAckStatus);
			this->panel_Display->Controls->Add(this->label33);
			this->panel_Display->Controls->Add(this->lbDisplayStroke);
			this->panel_Display->Controls->Add(this->lbDisplaySpeed);
			this->panel_Display->Controls->Add(this->lbDisplayEvent);
			this->panel_Display->Controls->Add(this->lbDisplayTime);
			this->panel_Display->Controls->Add(this->label20);
			this->panel_Display->Controls->Add(this->label21);
			this->panel_Display->Controls->Add(this->lbDisplayZone);
			this->panel_Display->Controls->Add(this->label26);
			this->panel_Display->Controls->Add(this->label31);
			this->panel_Display->Controls->Add(this->label32);
			this->panel_Display->Controls->Add(this->label13);
			this->panel_Display->Location = System::Drawing::Point(58, 188);
			this->panel_Display->Name = L"panel_Display";
			this->panel_Display->Size = System::Drawing::Size(200, 90);
			this->panel_Display->TabIndex = 1;
			// 
			// lbAckStatus
			// 
			this->lbAckStatus->AutoSize = true;
			this->lbAckStatus->Location = System::Drawing::Point(152, 71);
			this->lbAckStatus->Name = L"lbAckStatus";
			this->lbAckStatus->Size = System::Drawing::Size(19, 13);
			this->lbAckStatus->TabIndex = 21;
			this->lbAckStatus->Text = L"----";
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(84, 71);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(62, 13);
			this->label33->TabIndex = 20;
			this->label33->Text = L"Ack Status:";
			// 
			// lbDisplayStroke
			// 
			this->lbDisplayStroke->AutoSize = true;
			this->lbDisplayStroke->Location = System::Drawing::Point(44, 37);
			this->lbDisplayStroke->Name = L"lbDisplayStroke";
			this->lbDisplayStroke->Size = System::Drawing::Size(19, 13);
			this->lbDisplayStroke->TabIndex = 19;
			this->lbDisplayStroke->Text = L"----";
			// 
			// lbDisplaySpeed
			// 
			this->lbDisplaySpeed->AutoSize = true;
			this->lbDisplaySpeed->Location = System::Drawing::Point(44, 54);
			this->lbDisplaySpeed->Name = L"lbDisplaySpeed";
			this->lbDisplaySpeed->Size = System::Drawing::Size(19, 13);
			this->lbDisplaySpeed->TabIndex = 18;
			this->lbDisplaySpeed->Text = L"----";
			// 
			// lbDisplayEvent
			// 
			this->lbDisplayEvent->AutoSize = true;
			this->lbDisplayEvent->Location = System::Drawing::Point(156, 37);
			this->lbDisplayEvent->Name = L"lbDisplayEvent";
			this->lbDisplayEvent->Size = System::Drawing::Size(19, 13);
			this->lbDisplayEvent->TabIndex = 15;
			this->lbDisplayEvent->Text = L"----";
			// 
			// lbDisplayTime
			// 
			this->lbDisplayTime->AutoSize = true;
			this->lbDisplayTime->Location = System::Drawing::Point(156, 20);
			this->lbDisplayTime->Name = L"lbDisplayTime";
			this->lbDisplayTime->Size = System::Drawing::Size(19, 13);
			this->lbDisplayTime->TabIndex = 16;
			this->lbDisplayTime->Text = L"----";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(109, 37);
			this->label20->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(48, 13);
			this->label20->TabIndex = 10;
			this->label20->Text = L"Event #:";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(124, 20);
			this->label21->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(33, 13);
			this->label21->TabIndex = 13;
			this->label21->Text = L"Time:";
			// 
			// lbDisplayZone
			// 
			this->lbDisplayZone->AutoSize = true;
			this->lbDisplayZone->Location = System::Drawing::Point(44, 20);
			this->lbDisplayZone->Name = L"lbDisplayZone";
			this->lbDisplayZone->Size = System::Drawing::Size(19, 13);
			this->lbDisplayZone->TabIndex = 17;
			this->lbDisplayZone->Text = L"----";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(3, 54);
			this->label26->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(41, 13);
			this->label26->TabIndex = 12;
			this->label26->Text = L"Speed:";
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(3, 37);
			this->label31->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(41, 13);
			this->label31->TabIndex = 14;
			this->label31->Text = L"Stroke:";
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(9, 20);
			this->label32->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(35, 13);
			this->label32->TabIndex = 11;
			this->label32->Text = L"Zone:";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label13->Location = System::Drawing::Point(3, 2);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(130, 13);
			this->label13->TabIndex = 0;
			this->label13->Text = L"Current Racquet Rx Data:";
			// 
			// RacquetDisplay
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(794, 351);
			this->Controls->Add(this->panel_Display);
			this->Controls->Add(this->panel_Settings);
			this->Name = L"RacquetDisplay";
			this->Text = L"Racquet Display Simulator";
			this->panel_Settings->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabControl2->ResumeLayout(false);
			this->tabPage4->ResumeLayout(false);
			this->tabPage4->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTxResponseStrokes))->EndInit();
			this->tabPage5->ResumeLayout(false);
			this->tabPage5->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTxResponseZones))->EndInit();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTxResponsePrevious))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmPreviousEvent))->EndInit();
			this->tabPage7->ResumeLayout(false);
			this->tabControl3->ResumeLayout(false);
			this->tabPage8->ResumeLayout(false);
			this->tabPage8->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmOpponentGameScore))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMyGameScore))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->tabPage9->ResumeLayout(false);
			this->tabPage9->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmOpponentSetsWon))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMySetsWon))->EndInit();
			this->tabPage10->ResumeLayout(false);
			this->tabPage10->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMySetCount))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmOpponentSetCount))->EndInit();
			this->tabPage11->ResumeLayout(false);
			this->tabPage11->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMyGameCount))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmOpponentGameCount))->EndInit();
			this->tabPage17->ResumeLayout(false);
			this->tabControl5->ResumeLayout(false);
			this->tabPage18->ResumeLayout(false);
			this->groupBox_Calories->ResumeLayout(false);
			this->groupBox_Calories->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTotalCalories))->EndInit();
			this->groupBox_HR->ResumeLayout(false);
			this->groupBox_HR->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmAvgHR))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMaxHR))->EndInit();
			this->tabPage19->ResumeLayout(false);
			this->tabPage19->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmMaxRunSpeed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmAvgRunSpeed))->EndInit();
			this->tabPage20->ResumeLayout(false);
			this->tabPage20->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nmTotalDistance))->EndInit();
			this->tabPage6->ResumeLayout(false);
			this->tabControl4->ResumeLayout(false);
			this->tabPage12->ResumeLayout(false);
			this->tabPage12->PerformLayout();
			this->tabPage13->ResumeLayout(false);
			this->tabPage13->PerformLayout();
			this->panel_Display->ResumeLayout(false);
			this->panel_Display->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion



};