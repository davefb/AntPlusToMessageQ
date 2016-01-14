/*
This software is subject to the license described in the License.txt file 
included with this software distribution. You may not use this file except in compliance 
with this license.

Copyright (c) Dynastream Innovations Inc. 2012
All rights reserved.
*/
 

#pragma once

#define RACQUET_REV				1.0				// Device Profile Revision Number

public ref class AntPlusRacquet
{

public:
	// Channel Parameters
	static const UCHAR DEVICE_TYPE = 0x1A;
	static const UCHAR TX_TYPE = 5;
	static const USHORT MSG_PERIOD = 8192;	// 4 Hz

	// Data Pages
	static const UCHAR PAGE_STROKE_EVENT = 0x01;
	static const UCHAR PAGE_STROKE_COUNT = 0x02;
	static const UCHAR PAGE_PREVIOUS_STROKE_EVENT = 0x03;
	static const UCHAR PAGE_ZONE_COUNT = 0x04;
	static const UCHAR PAGE_EVENT_DATA = 0x20;
	static const UCHAR PAGE_SESSION_DATA = 0x21;
	static const UCHAR PAGE_LAP_DATA = 0x22;
	static const UCHAR PAGE_LENGTH_DATA = 0x23;
	static const UCHAR PAGE_METABOLIC_DATA = 0x24;
	static const UCHAR PAGE_SPEED_DATA = 0x25;
	static const UCHAR PAGE_DISTANCE_DATA = 0x26;

	// Reserved/Invalid/Special Values
	static const UCHAR RESERVED = 0xFF;
	static const USHORT UNKNOWN = 0xFFFF;
	static const ULONG UNKNOWN_ULONG = 0xFFFFFFFF;
	static const UCHAR NUMBER_OF_ZONES = 12;
	static const UCHAR NUMBER_OF_STROKE_TYPES = 7;

	// Bit Masks
	static const UCHAR BYTE_MASK = 0xFF;

	// Bit Shifts
	static const UCHAR NIBBLE_SHIFT = 4;
	static const UCHAR BYTE_SHIFT = 8;

	// Acknowledged Messages
	static const UCHAR ACK_FAIL = 0;
	static const UCHAR ACK_SUCCESS = 1;
	static const UCHAR ACK_RETRY = 2;
	static const UCHAR MAX_RETRIES = 5;		// defines the max number of times to retry before failing

	// Enumerations
	enum class StrokeTypes : UCHAR		// Stroke type definitions
	{
		NO_EVENT = 0,
		ALL_STROKES = 0,
		OTHER = 1, 
		SERVE = 2, 
		FOREHAND = 3, 
		BACKHAND = 4, 
		SMASH = 5, 
		UNKNOWN = 255,
	};
	enum class RacquetZones : UCHAR		// Racquet Zone definitions
	{
		MISS = 0,
		TOP_RIGHT = 1,
		MIDDLE_RIGHT = 2, 
		BOTTOM_RIGHT = 3,
		BOTTOM_CENTRE = 4,
		BOTTOM_LEFT = 5,
		MIDDLE_CENTRE = 6,
		MIDDLE_LEFT = 7,
		TOP_LEFT = 8, 
		TOP_CENTRE = 9,
		RIM_OTHER = 10,
		UNKNOWN = 63,
	};

	enum class ANTFS : UCHAR		// Racquet Zone definitions
	{
		NOT_SUPPORTED = 0,
		SUPPORTED = 1,
	};

	enum class FITEvents : UCHAR
	{
      TIMER = 0x00,
		SESSION = 0x08,
		USER_MARKER = 0x20,
		SPORT_POINT = 0x21,
	};
	enum class FITEventTypes : UCHAR
	{
		START = 0x00,
		STOP = 0x01,
		MARKER = 0x03,
	};
	enum class FITSports : UCHAR
	{
		TENNIS = 0x8,
	};
	enum class FITSubSports : UCHAR
	{
		WARM_UP = 21,
		MATCH = 22,
		EXERCISE = 23,
		CHALLENGE = 24,
	};

	// Error handling
	// Flags indicate errors causing the exception
	ref class Error : public System::Exception{
	public:
		BOOL	bUndefPage;			// Undefined page
		BOOL	bBadReserved;		// Invalid values on reserved fields
		BOOL	bUndefBatStatus;	// Undefined battery status value

		enum class Code : UCHAR		// Error code definitions
		{
			INVALID_RESERVED		
		};

		Error()
		{
			ClearFlags();
		}

		Error(Code eCode1_)
		{
			ClearFlags();
			SetFlags(eCode1_);		
		}

		Error(Code eCode1_, Code eCode2_)
		{		
			ClearFlags();
			SetFlags(eCode1_);	
			SetFlags(eCode2_);
		}


	private:
		void ClearFlags()
		{
			bUndefPage = FALSE;
			bBadReserved = FALSE;
			bUndefBatStatus = FALSE;
		}

		void SetFlags(Code eCode_)
		{
			switch(eCode_)
			{
			case Code::INVALID_RESERVED:
				bBadReserved = TRUE;
				break;
			default:
				break;

			}
		}
	};


public:
	// Current Stroke Event Data (Page 1)
	UCHAR	ucEventCount;	// Incremented with every stroke detected
	USHORT	usEventTime;	// Time that the last stroke event occurred
	UCHAR	ucEventType;	// Event/Stroke Type (in StrokeTypes enum)
	UCHAR	ucRacquetZone;	// Zone of the racquet event occurred on (RacquetZone enum)
	UCHAR   ucANTFS;        // Indicates whether ANT-FS is supported or not
	USHORT	usSpeed;		// Speed of the ball
	
	// Stroke Count Summary Data (Page 2)
	UCHAR	ucStrokeType1;	// Stroke Type of first subpaged item
	UCHAR	ucStrokeType2;	// Stroke Type of second subpaged item
	USHORT	usStrokeCount1;	// Stroke Count for Stroke Type 1
	USHORT	usStrokeCount2;	// Stroke Count for Stroke Type 2
	
	// Previous Stroke Event Data (Page 3)
	UCHAR	ucPreviousEventCount;	// Event count of the data included in this page
	USHORT	usPreviousEventTime;	// Accumulated time at the time of this event 
	UCHAR	ucPreviousEventType;	// Event/Stroke Type of this event
	UCHAR	ucPreviousRacquetZone;	// Zone of the racquet this event occurred on
	USHORT	usPreviousSpeed;		// Speed of the ball at this event
	
	// Racquet Zone Count Summary Data (Page 4)
	UCHAR	ucType;			// Stroke Type 
	UCHAR	ucZone;			// Racquet Zone
	USHORT	usCount;		// Stroke count for stroke type in zone 
	USHORT	usCountPlus1;	// Stroke count for stroke type in (zone + 1)

	// Event Data (Page 32)
	FITEvents eEvent;
	FITEventTypes eEventType;
	ULONG ulEventData;

	// Session Data (Page 33)
	FITSports eSport;
	FITSubSports eGameMode;
	USHORT usMySessionsWon;
	USHORT usOpponentSessionsWon;

	// Lap Data (Page 34)
	USHORT usMySetCount;
	USHORT usOpponentSetCount;

	// Length Data (Page 35)
	USHORT usMyGameCount;
	USHORT usOpponentGameCount;

   // Metabolic Data (Page 36)
   UCHAR ucMaxHeartRate;
   UCHAR ucAvgHeartRate;
   USHORT usTotalCalories;
   
   // Speed Data (Page 37)
   USHORT usMaxRunningSpeed;
   USHORT usAvgRunningSpeed;
   
   // Distance Data (Page 38)
   UINT32 uiTotalDistance;


public:
	AntPlusRacquet()
	{
	}

	~AntPlusRacquet()
	{
	}


public:

/**************************************************************************
 * AntPlusRacquet::Encode
 * 
 * Encodes common data
 * Exceptions are thrown when dealing with invalid data
 *
 * ucPageNum_: number of page to encode
 * pucTxBuffer_: pointer to the buffer that will store the encoded data
 * 
 * returns: N/A
 *
 **************************************************************************/
	void Encode(UCHAR ucPageNum_, UCHAR* pucTxBuffer_)
	{
		switch(ucPageNum_)
		{
			case PAGE_STROKE_EVENT:
				pucTxBuffer_[0] = ucPageNum_;								// Page # (1)
				pucTxBuffer_[1] = ucEventCount;								// Event Count of current stroke event			
				pucTxBuffer_[2] = usEventTime & BYTE_MASK;					// Lower byte of current event time
				pucTxBuffer_[3] = (usEventTime >> BYTE_SHIFT) & BYTE_MASK;	// Upper byte of current event time
				pucTxBuffer_[4] = ucEventType;								// Stroke type of this event
				pucTxBuffer_[5] = ucRacquetZone | (ucANTFS << 6);	        // Racquet zone that the event occurred on, ANT-FS capabilities of the device
				pucTxBuffer_[6] = usSpeed & BYTE_MASK;						// Lower byte of ball speed				
				pucTxBuffer_[7] = (usSpeed >> BYTE_SHIFT) & BYTE_MASK;		// Upper byte of ball speed			
				break;

			case PAGE_STROKE_COUNT:
				pucTxBuffer_[0] = ucPageNum_;									// Page # (2)
				pucTxBuffer_[1] = RESERVED;
				pucTxBuffer_[2] = ucStrokeType1;								// First stroke type subpaged in this page 			
				pucTxBuffer_[3] = ucStrokeType2;								// Second stroke type subpaged in this page
				pucTxBuffer_[4] = usStrokeCount1 & BYTE_MASK;					// Lower byte of stroke type 1 count
				pucTxBuffer_[5] = (usStrokeCount1 >> BYTE_SHIFT) & BYTE_MASK;	// Upper byte of stroke type 1 count
				pucTxBuffer_[6] = usStrokeCount2 & BYTE_MASK;					// Lower byte of stroke type 2 count
				pucTxBuffer_[7] = (usStrokeCount2 >> BYTE_SHIFT) & BYTE_MASK;	// Upper byte of stroke type 2 count
				break;

			case PAGE_PREVIOUS_STROKE_EVENT:
				pucTxBuffer_[0] = ucPageNum_;										// Page # (3)
				pucTxBuffer_[1] = ucPreviousEventCount;								// Event Count of requested stroke
				pucTxBuffer_[2] = usPreviousEventTime & BYTE_MASK;					// Lower byte of requested stroke time
				pucTxBuffer_[3] = (usPreviousEventTime >> BYTE_SHIFT) & BYTE_MASK;	// Upper byte of requested stroke time
				pucTxBuffer_[4] = ucPreviousEventType;								// Stroke type of requested event
				pucTxBuffer_[5] = ucPreviousRacquetZone;							// Racquet zone that the requeste event occurred on
				pucTxBuffer_[6] = usPreviousSpeed & BYTE_MASK;						// Lower byte of requested stroke ball speed
				pucTxBuffer_[7] = (usPreviousSpeed >> BYTE_SHIFT) & BYTE_MASK;		// Upper byte of requested stroke ball speed
				break;

			case PAGE_ZONE_COUNT:
				pucTxBuffer_[0] = ucPageNum_;								// Page # (4)
				pucTxBuffer_[1] = RESERVED;
				pucTxBuffer_[2] = ucType;									// Stroke type subpaged in this page
				pucTxBuffer_[3] = ucZone;									// Racquet zone subpaged in this page
				pucTxBuffer_[4] = usCount & BYTE_MASK;						// Lower byte of stroke count in zone
				pucTxBuffer_[5] = (usCount >> BYTE_SHIFT) & BYTE_MASK;		// Upper byte of stroke count in zone
				pucTxBuffer_[6] = usCountPlus1 & BYTE_MASK;					// Lower byte of stroke count in (zone + 1)
				pucTxBuffer_[7] = (usCountPlus1 >> BYTE_SHIFT)& BYTE_MASK;	// Upper byte of stroke count in (zone + 1)
				break;

			case PAGE_EVENT_DATA:
				pucTxBuffer_[0] = ucPageNum_;
				pucTxBuffer_[1] = RESERVED;
				pucTxBuffer_[2] = (UCHAR)eEvent;
				pucTxBuffer_[3] = (UCHAR)eEventType;
				pucTxBuffer_[4] = ulEventData & BYTE_MASK;
				pucTxBuffer_[5] = (ulEventData >> BYTE_SHIFT) & BYTE_MASK;
				pucTxBuffer_[6] = (ulEventData >> (2*BYTE_SHIFT)) & BYTE_MASK;
				pucTxBuffer_[7] = (ulEventData >> (3*BYTE_SHIFT)) & BYTE_MASK;
				break;

			case PAGE_SESSION_DATA:
				pucTxBuffer_[0] = ucPageNum_;
				pucTxBuffer_[1] = RESERVED;
				pucTxBuffer_[2] = (UCHAR)FITSports::TENNIS;
				pucTxBuffer_[3] = (UCHAR)eGameMode;
				pucTxBuffer_[4] = usMySessionsWon & BYTE_MASK;
				pucTxBuffer_[5] = (usMySessionsWon >> BYTE_SHIFT) & BYTE_MASK;
				pucTxBuffer_[6] = usOpponentSessionsWon & BYTE_MASK;
				pucTxBuffer_[7] = (usOpponentSessionsWon >> BYTE_SHIFT) & BYTE_MASK;
				break;

			case PAGE_LAP_DATA:
				pucTxBuffer_[0] = ucPageNum_;
				pucTxBuffer_[1] = RESERVED;
				pucTxBuffer_[2] = RESERVED;
				pucTxBuffer_[3] = RESERVED;
				pucTxBuffer_[4] = usMySetCount & BYTE_MASK;
				pucTxBuffer_[5] = (usMySetCount >> BYTE_SHIFT) & BYTE_MASK;
				pucTxBuffer_[6] = usOpponentSetCount & BYTE_MASK;
				pucTxBuffer_[7] = (usOpponentSetCount >> BYTE_SHIFT) & BYTE_MASK;
				break;

			case PAGE_LENGTH_DATA:
				pucTxBuffer_[0] = ucPageNum_;
				pucTxBuffer_[1] = RESERVED;
				pucTxBuffer_[2] = RESERVED;
				pucTxBuffer_[3] = RESERVED;
				pucTxBuffer_[4] = usMyGameCount & BYTE_MASK;
				pucTxBuffer_[5] = (usMyGameCount >> BYTE_SHIFT) & BYTE_MASK;
				pucTxBuffer_[6] = usOpponentGameCount & BYTE_MASK;
				pucTxBuffer_[7] = (usOpponentGameCount >> BYTE_SHIFT) & BYTE_MASK;
				break;

         case PAGE_METABOLIC_DATA:
				pucTxBuffer_[0] = ucPageNum_;
				pucTxBuffer_[1] = RESERVED;
				pucTxBuffer_[2] = RESERVED;
				pucTxBuffer_[3] = RESERVED;
				pucTxBuffer_[4] = ucMaxHeartRate;
				pucTxBuffer_[5] = ucAvgHeartRate;
				pucTxBuffer_[6] = usTotalCalories & BYTE_MASK;
            pucTxBuffer_[7] = (usTotalCalories >> BYTE_SHIFT) & BYTE_MASK;
            break;

         case PAGE_SPEED_DATA:
				pucTxBuffer_[0] = ucPageNum_;
				pucTxBuffer_[1] = RESERVED;
				pucTxBuffer_[2] = RESERVED;
				pucTxBuffer_[3] = RESERVED;
				pucTxBuffer_[4] = usMaxRunningSpeed & BYTE_MASK;
				pucTxBuffer_[5] = (usMaxRunningSpeed >> BYTE_SHIFT) & BYTE_MASK;
				pucTxBuffer_[6] = usAvgRunningSpeed & BYTE_MASK;
				pucTxBuffer_[7] = (usAvgRunningSpeed >> BYTE_SHIFT) & BYTE_MASK;
            break;

         case PAGE_DISTANCE_DATA:
				pucTxBuffer_[0] = ucPageNum_;
				pucTxBuffer_[1] = RESERVED;
				pucTxBuffer_[2] = RESERVED;
				pucTxBuffer_[3] = RESERVED;
				pucTxBuffer_[4] = uiTotalDistance & BYTE_MASK;
				pucTxBuffer_[5] = (uiTotalDistance >> BYTE_SHIFT) & BYTE_MASK;
				pucTxBuffer_[6] = (uiTotalDistance >> (BYTE_SHIFT * 2)) & BYTE_MASK;
				pucTxBuffer_[7] = (uiTotalDistance >> (BYTE_SHIFT * 3)) & BYTE_MASK;
            break;

			default:
				break;
		}
	}


/**************************************************************************
 * AntPlusRacquet::Decode
 * 
 * Decodes received Racquet Data Pages
 * Exceptions are thrown when dealing with invalid data
 *
 * pucRxBuffer_: pointer to the buffer containing the received data
 * 
 * returns: N/A
 *
 **************************************************************************/
	void Decode(UCHAR* pucRxBuffer_)
	{
		switch(pucRxBuffer_[0])
		{
			case PAGE_STROKE_EVENT:
				ucEventCount = pucRxBuffer_[1];
				usEventTime = pucRxBuffer_[2] | (pucRxBuffer_[3] << BYTE_SHIFT);
				ucEventType = pucRxBuffer_[4];
				ucRacquetZone = pucRxBuffer_[5] & 0x3F;
				ucANTFS = ((pucRxBuffer_[5]  & 0xC0) >> 6);
				usSpeed = pucRxBuffer_[6] | (pucRxBuffer_[7] << BYTE_SHIFT);
				break;

			case PAGE_STROKE_COUNT: 
				ucStrokeType1 = pucRxBuffer_[2];
				ucStrokeType2 = pucRxBuffer_[3];
				usStrokeCount1 = pucRxBuffer_[4] | (pucRxBuffer_[5] << BYTE_SHIFT);
				usStrokeCount2 = pucRxBuffer_[6] | (pucRxBuffer_[7] << BYTE_SHIFT);
				break;

			case PAGE_PREVIOUS_STROKE_EVENT:
				ucPreviousEventCount = pucRxBuffer_[1];
				usPreviousEventTime = pucRxBuffer_[2] | (pucRxBuffer_[3] << BYTE_SHIFT);
				ucPreviousEventType = pucRxBuffer_[4];
				ucPreviousRacquetZone = pucRxBuffer_[5];
				usPreviousSpeed = pucRxBuffer_[6] | (pucRxBuffer_[7] << BYTE_SHIFT);
				break;

			case PAGE_ZONE_COUNT:
				ucType = pucRxBuffer_[2];
				ucZone = pucRxBuffer_[3];
				usCount = pucRxBuffer_[4] | (pucRxBuffer_[5] << BYTE_SHIFT);
				usCountPlus1 = pucRxBuffer_[6] | (pucRxBuffer_[7] << BYTE_SHIFT);
				break;

			case PAGE_EVENT_DATA:
				eEvent = (FITEvents)(pucRxBuffer_[2]);
				eEventType = (FITEventTypes)(pucRxBuffer_[3]);
				ulEventData = pucRxBuffer_[4] | (pucRxBuffer_[5] << BYTE_SHIFT)
					| (pucRxBuffer_[6] << (2*BYTE_SHIFT)) | (pucRxBuffer_[7] << (3*BYTE_SHIFT));
				break;

			case PAGE_SESSION_DATA:
				eSport = (FITSports)(pucRxBuffer_[2]);
				eGameMode = (FITSubSports)(pucRxBuffer_[3]);
				usMySessionsWon = pucRxBuffer_[4] | (pucRxBuffer_[5] << BYTE_SHIFT);
				usOpponentSessionsWon = pucRxBuffer_[6] | (pucRxBuffer_[7] << BYTE_SHIFT);
				break;

			case PAGE_LAP_DATA:
				usMySetCount = pucRxBuffer_[4] | (pucRxBuffer_[5] << BYTE_SHIFT);
				usOpponentSetCount = pucRxBuffer_[6] | (pucRxBuffer_[7] << BYTE_SHIFT);
				break;

			case PAGE_LENGTH_DATA:
				usMyGameCount = pucRxBuffer_[4] | (pucRxBuffer_[5] << BYTE_SHIFT);
				usOpponentGameCount = pucRxBuffer_[6] | (pucRxBuffer_[7] << BYTE_SHIFT);
				break;

         case PAGE_METABOLIC_DATA:
				ucMaxHeartRate = pucRxBuffer_[4];
            ucAvgHeartRate = pucRxBuffer_[5];
            usTotalCalories = pucRxBuffer_[6] | (pucRxBuffer_[7] << BYTE_SHIFT);
            break;

         case PAGE_SPEED_DATA:
            usMaxRunningSpeed = pucRxBuffer_[4] | (pucRxBuffer_[5] << BYTE_SHIFT);
            usAvgRunningSpeed = pucRxBuffer_[6] | (pucRxBuffer_[5] << BYTE_SHIFT);
            break;

         case PAGE_DISTANCE_DATA:
            uiTotalDistance = pucRxBuffer_[4] 
                            | (pucRxBuffer_[5] << BYTE_SHIFT)
                            | (pucRxBuffer_[6] << (BYTE_SHIFT * 2)) 
                            | (pucRxBuffer_[7] << (BYTE_SHIFT * 3));
            break;

			default:
				break;			
		}
	}
};
