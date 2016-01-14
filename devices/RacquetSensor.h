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
#include "antplus_racquet.h"	
#include "antplus_common.h"				
#include "types.h"
#include "antdefines.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Timers;

public ref class RacquetSensor : public System::Windows::Forms::Form, public ISimBase{
public:
	// The constructor may include the following parameters:
	// System::Timers::Timer^ channelTimer:  Timer handle, if using a timer (not required in most cases, since getTimerInterval gets called after onTimerTock on each tock)
	// dRequestAckMsg^ channelAckMsg:  Handle to delegate function to send acknowledged messages (only needed if the device needs to send acknowledged messages)
	// These parameters may or may not be included in the constructor, as needed
		RacquetSensor(System::Timers::Timer^ channelTimer, dRequestAckMsg^ channelAckMsg){
			InitializeComponent();
			RacquetData = gcnew AntPlusRacquet();
			Common = gcnew CommonData();
			requestAckMsg = channelAckMsg;	
			timerHandle = channelTimer;		
			InitializeSim();
		}

		~RacquetSensor(){
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
	virtual UCHAR getDeviceType(){return AntPlusRacquet::DEVICE_TYPE;}  	// Eg. BikePower::DEVICE_TYPE, SDM_DEVICE_TYPE
	virtual UCHAR getTransmissionType(){return AntPlusRacquet::TX_TYPE;} 	// Eg. BikePower::TX_TYPE, SDM_TX_TYPE, 0 (for receivers, if wildcarding)
	virtual USHORT getTransmitPeriod(){return AntPlusRacquet::MSG_PERIOD;}	// Eg. BikePower::MSG_PERIOD, SDM_MSG_PERIOD
	virtual DOUBLE getTimerInterval(){return (double) ulTimerInterval;}	// Set to any value for receivers not using the timer.
	virtual System::Windows::Forms::Panel^ getSimSettingsPanel(){return this->panel_Settings;}
	virtual System::Windows::Forms::Panel^ getSimTranslatedDisplay(){return this->panel_Display;}

// Optional methods (not required by interface class)
private:
	// Initialize simulation
	void InitializeSim();						

	// Device Specific Functions
	void HandleTransmit(UCHAR* pucTxBuffer_);	// Handles data page transmission 
	void UpdateDisplay();						// Updates the UI
	void HandleReceive(UCHAR* pucRxBuffer_);	// Handles any back channel data pages
	void AddEvent();							// Adds event to previous event array

	// GUI Specific Functions 
	System::Void zoneOne_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneTwo_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneThree_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneFour_Click(System::Object^  sender, System::EventArgs^  e); 
	System::Void zoneFive_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneSix_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneSeven_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneEight_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneNine_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneOther_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneUnknown_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void zoneMiss_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void ckInvalidSpeed_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbEventCount_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbStrokeType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbRacquetZone_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void cbZoneStrokes_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void btUpdateCommon_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void cbInvalidSerial_CheckedChanged(System::Object^  sender, System::EventArgs^  e); 
	static System::Void timerSession_Tick(System::Object^  sender, System::Timers::ElapsedEventArgs^  e);
   System::Void checkMpS_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
   System::Void comboBoxBatStatus_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
   System::Void checkBox_Bat_Voltage_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
   System::Void numericCoarseVoltage_ValueChanged(System::Object^  sender, System::EventArgs^  e);
   System::Void numericFractionalVoltage_ValueChanged(System::Object^  sender, System::EventArgs^  e);
   System::Void radioResolution2_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
   System::Void buttonUpdateTime_Click(System::Object^  sender, System::EventArgs^  e);
   System::Void checkANTFS_CheckedChanged (System::Object^ sender, System::EventArgs^ e);

private:
	dRequestAckMsg^ requestAckMsg;		// Delegate handle (ACK messages, only if used)
	System::Timers::Timer^ timerHandle;	// Timer handle (only if there is a timer)
	ULONG  ulTimerInterval;				// Timer interval between simulated events (only if there is a timer)
	bool bTxRequest;
	UCHAR ucRequestCount;
	System::Timers::Timer^ timerSession;

	// ANT Plus objects
	AntPlusRacquet^ RacquetData;
	CommonData^		Common; 
	System::DateTime startTime;
	System::DateTime lastTime;
	static int time;
   ULONG ulTotalTime;

	array<array<USHORT>^>^ zones;

public: value struct Event{
	Double EventCount;
	USHORT EventTime;
	UCHAR EventType;
	UCHAR RacquetZone;
	USHORT BallSpeed;
	};

private: array<Event^> ^ StrokeEvents;	// Holds the user initiated stroke events


private: System::Windows::Forms::Panel^  panel_Settings;
private: System::Windows::Forms::Panel^  panel_Display;
private: System::Windows::Forms::TabControl^  tabControl1;
private: System::Windows::Forms::TabPage^  tabPage1;
private: System::Windows::Forms::TabPage^  tabPage2;
private: System::Windows::Forms::Button^  zoneEight;
private: System::Windows::Forms::Button^  zoneFive;
private: System::Windows::Forms::Button^  zoneFour;
private: System::Windows::Forms::Button^  zoneSix;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Button^  zoneOne;
private: System::Windows::Forms::Button^  zoneSeven;
private: System::Windows::Forms::Label^  lbTime;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::ComboBox^  comboType;
private: System::Windows::Forms::NumericUpDown^  valueSpeed;
private: System::Windows::Forms::CheckBox^  ckInvalidSpeed;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Button^  zoneUnknown;
private: System::Windows::Forms::Button^  zoneOther;
private: System::Windows::Forms::Button^  zoneTwo;
private: System::Windows::Forms::GroupBox^  groupBox2;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::ComboBox^  cbStrokeType;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::ComboBox^  cbZoneStrokes;
private: System::Windows::Forms::ComboBox^  cbRacquetZone;
private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::Label^  label11;
private: System::Windows::Forms::TabPage^  tabPage3;
private: System::Windows::Forms::Button^  zoneThree;
private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::ComboBox^  cbEventCount;
private: System::Windows::Forms::Label^  lbEventTime;
private: System::Windows::Forms::Label^  lbBallSpeed;
private: System::Windows::Forms::Label^  lbStrokeType;
private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::Button^  zoneNine;
private: System::Windows::Forms::Label^  lbRacquetZone;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::Button^  zoneMiss;
private: System::Windows::Forms::Label^  lbZoneStrokes;
private: System::Windows::Forms::Label^  lbStrokes;
private: System::Windows::Forms::CheckBox^  cbUnknownZoneCount;
private: System::Windows::Forms::CheckBox^  cbUnknownStrokeCount;
private: System::Windows::Forms::TabPage^  tabPage4;
private: System::Windows::Forms::CheckBox^  cbInvalidSerial;
private: System::Windows::Forms::Button^  btUpdateCommon;
private: System::Windows::Forms::Label^  lbCommonError;
private: System::Windows::Forms::TextBox^  tbHardwareVer;
private: System::Windows::Forms::TextBox^  tbModelNum;
private: System::Windows::Forms::Label^  label_Glb_HardwareVer;
private: System::Windows::Forms::Label^  label_Glb_ManfID;
private: System::Windows::Forms::TextBox^  tbManfId;
private: System::Windows::Forms::Label^  label_Glb_SoftwareVer;
private: System::Windows::Forms::Label^  label_Glb_ModelNum;
private: System::Windows::Forms::TextBox^  tbSerialNumber;
private: System::Windows::Forms::TextBox^  tbSoftwareVer;
private: System::Windows::Forms::Label^  label_Glb_SerialNum;
private: System::Windows::Forms::Label^  label18;
private: System::Windows::Forms::Label^  lbDisplayStroke;
private: System::Windows::Forms::Label^  lbDisplaySpeed;
private: System::Windows::Forms::Label^  lbDisplayZone;
private: System::Windows::Forms::Label^  lbDisplayTime;
private: System::Windows::Forms::Label^  lbDisplayEvent;
private: System::Windows::Forms::Label^  label23;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::TabPage^  tabPage5;
private: System::Windows::Forms::TabControl^  tabControl2;
private: System::Windows::Forms::Label^  lbOpponentScore;
private: System::Windows::Forms::Label^  lbMyScore;
private: System::Windows::Forms::Label^  lbUserMarker;
private: System::Windows::Forms::Label^  label24;
private: System::Windows::Forms::Label^  lbGameMode;
private: System::Windows::Forms::Label^  label27;
private: System::Windows::Forms::Label^  lbOpponentSessionsWon;
private: System::Windows::Forms::Label^  lbMySessionsWon;
private: System::Windows::Forms::Label^  lbMySetCount;
private: System::Windows::Forms::Label^  lbOpponentGameCount;
private: System::Windows::Forms::Label^  lbMyGameCount;
private: System::Windows::Forms::Label^  lbOpponentSetCount;
public: static System::Windows::Forms::Label^  lbSessionTime;
private: 

private: 

private: 

private: 

private: System::Windows::Forms::Label^  label32;
private: System::Windows::Forms::Panel^  panel1;
private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Label^  label34;
private: System::Windows::Forms::Label^  label33;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::Label^  lbGameModeText;

private: System::Windows::Forms::RadioButton^  checkMpS;
private: System::Windows::Forms::TabControl^  tabControl3;
private: System::Windows::Forms::TabPage^  tabPage6;
private: System::Windows::Forms::TabPage^  tabPage7;
private: System::Windows::Forms::Label^  label28;
private: System::Windows::Forms::ComboBox^  comboBoxBatStatus;
private: System::Windows::Forms::Label^  label30;
private: System::Windows::Forms::Label^  label29;
private: System::Windows::Forms::NumericUpDown^  numericCoarseVoltage;
private: System::Windows::Forms::NumericUpDown^  numericFractionalVoltage;
private: System::Windows::Forms::CheckBox^  checkBox_Bat_Voltage;
private: System::Windows::Forms::Label^  labelElpTime;
private: System::Windows::Forms::Label^  label_Bat_ElpTime;
private: System::Windows::Forms::RadioButton^  radioResolution2;
private: System::Windows::Forms::RadioButton^  radioResolution16;
private: System::Windows::Forms::TextBox^  textBoxTime;
private: System::Windows::Forms::Button^  buttonUpdateTime;
private: System::Windows::Forms::Label^  labelTimeError;
private: System::Windows::Forms::Label^  label35;
private: System::Windows::Forms::TabPage^  tabPage10;




private: System::Windows::Forms::Label^  label45;
private: System::Windows::Forms::Label^  lbMaxHRDisplay;
private: System::Windows::Forms::Label^  lbMaxRunSpdDisplay;




private: System::Windows::Forms::Label^  label48;
private: System::Windows::Forms::Label^  label50;
private: System::Windows::Forms::Label^  lbTotDistanceDisplay;





private: System::Windows::Forms::Label^  label36;
private: System::Windows::Forms::Label^  lbAvgRunSpdDisplay;

private: System::Windows::Forms::Label^  lbAvgHRDisplay;



private: System::Windows::Forms::Label^  label49;
private: System::Windows::Forms::Label^  lbTotCaloriesDisplay;

private: System::Windows::Forms::Label^  label37;
private: System::Windows::Forms::CheckBox^  checkANTFS;
private: System::Windows::Forms::RadioButton^  checkKMpH;




private: System::ComponentModel::IContainer^  components;

		 /// <summary>
		/// Required designer variable.a
		/// </summary>


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
			this->checkMpS = (gcnew System::Windows::Forms::RadioButton());
			this->zoneMiss = (gcnew System::Windows::Forms::Button());
			this->zoneNine = (gcnew System::Windows::Forms::Button());
			this->zoneThree = (gcnew System::Windows::Forms::Button());
			this->lbTime = (gcnew System::Windows::Forms::Label());
			this->zoneTwo = (gcnew System::Windows::Forms::Button());
			this->zoneUnknown = (gcnew System::Windows::Forms::Button());
			this->zoneOther = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->zoneOne = (gcnew System::Windows::Forms::Button());
			this->ckInvalidSpeed = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->zoneSix = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->valueSpeed = (gcnew System::Windows::Forms::NumericUpDown());
			this->comboType = (gcnew System::Windows::Forms::ComboBox());
			this->zoneFive = (gcnew System::Windows::Forms::Button());
			this->zoneFour = (gcnew System::Windows::Forms::Button());
			this->zoneEight = (gcnew System::Windows::Forms::Button());
			this->zoneSeven = (gcnew System::Windows::Forms::Button());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->cbUnknownZoneCount = (gcnew System::Windows::Forms::CheckBox());
			this->lbZoneStrokes = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->cbZoneStrokes = (gcnew System::Windows::Forms::ComboBox());
			this->cbRacquetZone = (gcnew System::Windows::Forms::ComboBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->cbUnknownStrokeCount = (gcnew System::Windows::Forms::CheckBox());
			this->lbStrokes = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->cbStrokeType = (gcnew System::Windows::Forms::ComboBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->lbRacquetZone = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->lbEventTime = (gcnew System::Windows::Forms::Label());
			this->lbBallSpeed = (gcnew System::Windows::Forms::Label());
			this->lbStrokeType = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->cbEventCount = (gcnew System::Windows::Forms::ComboBox());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->lbUserMarker = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->lbOpponentSetCount = (gcnew System::Windows::Forms::Label());
			this->lbGameModeText = (gcnew System::Windows::Forms::Label());
			this->lbOpponentSessionsWon = (gcnew System::Windows::Forms::Label());
			this->lbMySetCount = (gcnew System::Windows::Forms::Label());
			this->lbOpponentGameCount = (gcnew System::Windows::Forms::Label());
			this->lbMySessionsWon = (gcnew System::Windows::Forms::Label());
			this->lbOpponentScore = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->lbMyScore = (gcnew System::Windows::Forms::Label());
			this->lbMyGameCount = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->tabControl2 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage10 = (gcnew System::Windows::Forms::TabPage());
			this->lbTotCaloriesDisplay = (gcnew System::Windows::Forms::Label());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->lbTotDistanceDisplay = (gcnew System::Windows::Forms::Label());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->lbAvgRunSpdDisplay = (gcnew System::Windows::Forms::Label());
			this->lbAvgHRDisplay = (gcnew System::Windows::Forms::Label());
			this->label45 = (gcnew System::Windows::Forms::Label());
			this->lbMaxHRDisplay = (gcnew System::Windows::Forms::Label());
			this->lbMaxRunSpdDisplay = (gcnew System::Windows::Forms::Label());
			this->label48 = (gcnew System::Windows::Forms::Label());
			this->label49 = (gcnew System::Windows::Forms::Label());
			this->label50 = (gcnew System::Windows::Forms::Label());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl3 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage6 = (gcnew System::Windows::Forms::TabPage());
			this->cbInvalidSerial = (gcnew System::Windows::Forms::CheckBox());
			this->btUpdateCommon = (gcnew System::Windows::Forms::Button());
			this->lbCommonError = (gcnew System::Windows::Forms::Label());
			this->label_Glb_SerialNum = (gcnew System::Windows::Forms::Label());
			this->tbManfId = (gcnew System::Windows::Forms::TextBox());
			this->label_Glb_ManfID = (gcnew System::Windows::Forms::Label());
			this->tbHardwareVer = (gcnew System::Windows::Forms::TextBox());
			this->label_Glb_ModelNum = (gcnew System::Windows::Forms::Label());
			this->tbSoftwareVer = (gcnew System::Windows::Forms::TextBox());
			this->tbModelNum = (gcnew System::Windows::Forms::TextBox());
			this->label_Glb_HardwareVer = (gcnew System::Windows::Forms::Label());
			this->tbSerialNumber = (gcnew System::Windows::Forms::TextBox());
			this->label_Glb_SoftwareVer = (gcnew System::Windows::Forms::Label());
			this->tabPage7 = (gcnew System::Windows::Forms::TabPage());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->radioResolution2 = (gcnew System::Windows::Forms::RadioButton());
			this->comboBoxBatStatus = (gcnew System::Windows::Forms::ComboBox());
			this->labelTimeError = (gcnew System::Windows::Forms::Label());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->radioResolution16 = (gcnew System::Windows::Forms::RadioButton());
			this->buttonUpdateTime = (gcnew System::Windows::Forms::Button());
			this->textBoxTime = (gcnew System::Windows::Forms::TextBox());
			this->labelElpTime = (gcnew System::Windows::Forms::Label());
			this->label_Bat_ElpTime = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->numericCoarseVoltage = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericFractionalVoltage = (gcnew System::Windows::Forms::NumericUpDown());
			this->checkBox_Bat_Voltage = (gcnew System::Windows::Forms::CheckBox());
			this->lbGameMode = (gcnew System::Windows::Forms::Label());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->panel_Display = (gcnew System::Windows::Forms::Panel());
			this->lbSessionTime = (gcnew System::Windows::Forms::Label());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->lbDisplayStroke = (gcnew System::Windows::Forms::Label());
			this->lbDisplaySpeed = (gcnew System::Windows::Forms::Label());
			this->lbDisplayEvent = (gcnew System::Windows::Forms::Label());
			this->lbDisplayTime = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->lbDisplayZone = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->checkANTFS = (gcnew System::Windows::Forms::CheckBox());
			this->checkKMpH = (gcnew System::Windows::Forms::RadioButton());
			this->panel_Settings->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->valueSpeed))->BeginInit();
			this->tabPage2->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->tabPage5->SuspendLayout();
			this->panel1->SuspendLayout();
			this->tabPage10->SuspendLayout();
			this->tabPage4->SuspendLayout();
			this->tabControl3->SuspendLayout();
			this->tabPage6->SuspendLayout();
			this->tabPage7->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericCoarseVoltage))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericFractionalVoltage))->BeginInit();
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
			this->tabControl1->Controls->Add(this->tabPage5);
			this->tabControl1->Controls->Add(this->tabPage10);
			this->tabControl1->Controls->Add(this->tabPage4);
			this->tabControl1->Location = System::Drawing::Point(3, 3);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(394, 134);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->checkANTFS);
			this->tabPage1->Controls->Add(this->checkKMpH);
			this->tabPage1->Controls->Add(this->checkMpS);
			this->tabPage1->Controls->Add(this->zoneMiss);
			this->tabPage1->Controls->Add(this->zoneNine);
			this->tabPage1->Controls->Add(this->zoneThree);
			this->tabPage1->Controls->Add(this->lbTime);
			this->tabPage1->Controls->Add(this->zoneTwo);
			this->tabPage1->Controls->Add(this->zoneUnknown);
			this->tabPage1->Controls->Add(this->zoneOther);
			this->tabPage1->Controls->Add(this->label6);
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->zoneOne);
			this->tabPage1->Controls->Add(this->ckInvalidSpeed);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->zoneSix);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Controls->Add(this->valueSpeed);
			this->tabPage1->Controls->Add(this->comboType);
			this->tabPage1->Controls->Add(this->zoneFive);
			this->tabPage1->Controls->Add(this->zoneFour);
			this->tabPage1->Controls->Add(this->zoneEight);
			this->tabPage1->Controls->Add(this->zoneSeven);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(386, 108);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Current Event";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// checkMpS
			// 
			this->checkMpS->AutoSize = true;
			this->checkMpS->Checked = true;
			this->checkMpS->Location = System::Drawing::Point(245, 70);
			this->checkMpS->Name = L"checkMpS";
			this->checkMpS->Size = System::Drawing::Size(43, 17);
			this->checkMpS->TabIndex = 25;
			this->checkMpS->TabStop = true;
			this->checkMpS->Text = L"m/s";
			this->checkMpS->UseVisualStyleBackColor = true;
			this->checkMpS->CheckedChanged += gcnew System::EventHandler(this, &RacquetSensor::checkMpS_CheckedChanged);
			// 
			// zoneMiss
			// 
			this->zoneMiss->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneMiss->Location = System::Drawing::Point(119, 20);
			this->zoneMiss->Name = L"zoneMiss";
			this->zoneMiss->Size = System::Drawing::Size(24, 60);
			this->zoneMiss->TabIndex = 24;
			this->zoneMiss->Text = L"M\r\ni\r\ns\r\ns";
			this->zoneMiss->UseVisualStyleBackColor = true;
			this->zoneMiss->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneMiss_Click);
			// 
			// zoneNine
			// 
			this->zoneNine->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneNine->Location = System::Drawing::Point(26, 40);
			this->zoneNine->Margin = System::Windows::Forms::Padding(0);
			this->zoneNine->Name = L"zoneNine";
			this->zoneNine->Size = System::Drawing::Size(30, 20);
			this->zoneNine->TabIndex = 23;
			this->zoneNine->Text = L"9";
			this->zoneNine->UseVisualStyleBackColor = true;
			this->zoneNine->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneNine_Click);
			// 
			// zoneThree
			// 
			this->zoneThree->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneThree->Location = System::Drawing::Point(86, 20);
			this->zoneThree->Margin = System::Windows::Forms::Padding(0);
			this->zoneThree->Name = L"zoneThree";
			this->zoneThree->Size = System::Drawing::Size(30, 20);
			this->zoneThree->TabIndex = 22;
			this->zoneThree->Text = L"3";
			this->zoneThree->UseVisualStyleBackColor = true;
			this->zoneThree->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneThree_Click);
			// 
			// lbTime
			// 
			this->lbTime->AutoSize = true;
			this->lbTime->Location = System::Drawing::Point(234, 89);
			this->lbTime->Name = L"lbTime";
			this->lbTime->Size = System::Drawing::Size(19, 13);
			this->lbTime->TabIndex = 18;
			this->lbTime->Text = L"----";
			// 
			// zoneTwo
			// 
			this->zoneTwo->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneTwo->Location = System::Drawing::Point(56, 20);
			this->zoneTwo->Margin = System::Windows::Forms::Padding(0);
			this->zoneTwo->Name = L"zoneTwo";
			this->zoneTwo->Size = System::Drawing::Size(30, 20);
			this->zoneTwo->TabIndex = 21;
			this->zoneTwo->Text = L"2";
			this->zoneTwo->UseVisualStyleBackColor = true;
			this->zoneTwo->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneTwo_Click);
			// 
			// zoneUnknown
			// 
			this->zoneUnknown->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneUnknown->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->zoneUnknown->Location = System::Drawing::Point(71, 83);
			this->zoneUnknown->Name = L"zoneUnknown";
			this->zoneUnknown->Size = System::Drawing::Size(45, 20);
			this->zoneUnknown->TabIndex = 20;
			this->zoneUnknown->Text = L"UNK";
			this->zoneUnknown->UseVisualStyleBackColor = true;
			this->zoneUnknown->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneUnknown_Click);
			// 
			// zoneOther
			// 
			this->zoneOther->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneOther->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->zoneOther->Location = System::Drawing::Point(26, 83);
			this->zoneOther->Name = L"zoneOther";
			this->zoneOther->Size = System::Drawing::Size(45, 20);
			this->zoneOther->TabIndex = 19;
			this->zoneOther->Text = L"Other";
			this->zoneOther->UseVisualStyleBackColor = true;
			this->zoneOther->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneOther_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(167, 89);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(64, 13);
			this->label6->TabIndex = 17;
			this->label6->Text = L"Event Time:";
			// 
			// label5
			// 
			this->label5->Location = System::Drawing::Point(144, 20);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(19, 75);
			this->label5->TabIndex = 16;
			this->label5->Text = L"B\r\nO\r\nT\r\nT\r\nO\r\nM";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label4
			// 
			this->label4->Location = System::Drawing::Point(6, 22);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(17, 73);
			this->label4->TabIndex = 15;
			this->label4->Text = L"T\r\nO\r\nP";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label3
			// 
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(26, -3);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(90, 23);
			this->label3->TabIndex = 14;
			this->label3->Text = L"Racquet Zone";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// zoneOne
			// 
			this->zoneOne->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneOne->Location = System::Drawing::Point(26, 20);
			this->zoneOne->Margin = System::Windows::Forms::Padding(0);
			this->zoneOne->Name = L"zoneOne";
			this->zoneOne->Size = System::Drawing::Size(30, 20);
			this->zoneOne->TabIndex = 9;
			this->zoneOne->Text = L"1";
			this->zoneOne->UseVisualStyleBackColor = true;
			this->zoneOne->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneOne_Click);
			// 
			// ckInvalidSpeed
			// 
			this->ckInvalidSpeed->AutoSize = true;
			this->ckInvalidSpeed->Location = System::Drawing::Point(326, 45);
			this->ckInvalidSpeed->Name = L"ckInvalidSpeed";
			this->ckInvalidSpeed->Size = System::Drawing::Size(60, 17);
			this->ckInvalidSpeed->TabIndex = 13;
			this->ckInvalidSpeed->Text = L"Invalid ";
			this->ckInvalidSpeed->UseVisualStyleBackColor = true;
			this->ckInvalidSpeed->CheckedChanged += gcnew System::EventHandler(this, &RacquetSensor::ckInvalidSpeed_CheckedChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(178, 46);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(61, 13);
			this->label2->TabIndex = 12;
			this->label2->Text = L"Ball Speed:";
			// 
			// zoneSix
			// 
			this->zoneSix->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneSix->Location = System::Drawing::Point(56, 40);
			this->zoneSix->Margin = System::Windows::Forms::Padding(0);
			this->zoneSix->Name = L"zoneSix";
			this->zoneSix->Size = System::Drawing::Size(30, 20);
			this->zoneSix->TabIndex = 4;
			this->zoneSix->Text = L"6";
			this->zoneSix->UseVisualStyleBackColor = true;
			this->zoneSix->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneSix_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(171, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(68, 13);
			this->label1->TabIndex = 11;
			this->label1->Text = L"Stroke Type:";
			// 
			// valueSpeed
			// 
			this->valueSpeed->DecimalPlaces = 2;
			this->valueSpeed->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->valueSpeed->Location = System::Drawing::Point(245, 44);
			this->valueSpeed->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65534, 0, 0, 131072});
			this->valueSpeed->Name = L"valueSpeed";
			this->valueSpeed->Size = System::Drawing::Size(75, 20);
			this->valueSpeed->TabIndex = 10;
			this->valueSpeed->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			// 
			// comboType
			// 
			this->comboType->FormattingEnabled = true;
			this->comboType->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"No Event", L"Other", L"Serve", L"Forehand", L"Backhand", 
				L"Smash", L"Unknown"});
			this->comboType->Location = System::Drawing::Point(245, 17);
			this->comboType->Name = L"comboType";
			this->comboType->Size = System::Drawing::Size(75, 21);
			this->comboType->TabIndex = 9;
			this->comboType->Text = L"Serve";
			// 
			// zoneFive
			// 
			this->zoneFive->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneFive->Location = System::Drawing::Point(86, 60);
			this->zoneFive->Margin = System::Windows::Forms::Padding(0);
			this->zoneFive->Name = L"zoneFive";
			this->zoneFive->Size = System::Drawing::Size(30, 20);
			this->zoneFive->TabIndex = 6;
			this->zoneFive->Text = L"5";
			this->zoneFive->UseVisualStyleBackColor = true;
			this->zoneFive->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneFive_Click);
			// 
			// zoneFour
			// 
			this->zoneFour->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneFour->Location = System::Drawing::Point(86, 40);
			this->zoneFour->Margin = System::Windows::Forms::Padding(0);
			this->zoneFour->Name = L"zoneFour";
			this->zoneFour->Size = System::Drawing::Size(30, 20);
			this->zoneFour->TabIndex = 5;
			this->zoneFour->Text = L"4";
			this->zoneFour->UseVisualStyleBackColor = true;
			this->zoneFour->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneFour_Click);
			// 
			// zoneEight
			// 
			this->zoneEight->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneEight->Location = System::Drawing::Point(26, 60);
			this->zoneEight->Margin = System::Windows::Forms::Padding(0);
			this->zoneEight->Name = L"zoneEight";
			this->zoneEight->Size = System::Drawing::Size(30, 20);
			this->zoneEight->TabIndex = 8;
			this->zoneEight->Text = L"8";
			this->zoneEight->UseVisualStyleBackColor = true;
			this->zoneEight->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneEight_Click);
			// 
			// zoneSeven
			// 
			this->zoneSeven->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->zoneSeven->Location = System::Drawing::Point(56, 60);
			this->zoneSeven->Margin = System::Windows::Forms::Padding(0);
			this->zoneSeven->Name = L"zoneSeven";
			this->zoneSeven->Size = System::Drawing::Size(30, 20);
			this->zoneSeven->TabIndex = 7;
			this->zoneSeven->Text = L"7";
			this->zoneSeven->UseVisualStyleBackColor = true;
			this->zoneSeven->Click += gcnew System::EventHandler(this, &RacquetSensor::zoneSeven_Click);
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->groupBox2);
			this->tabPage2->Controls->Add(this->groupBox1);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(386, 108);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Stroke Summary";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->cbUnknownZoneCount);
			this->groupBox2->Controls->Add(this->lbZoneStrokes);
			this->groupBox2->Controls->Add(this->label12);
			this->groupBox2->Controls->Add(this->label11);
			this->groupBox2->Controls->Add(this->cbZoneStrokes);
			this->groupBox2->Controls->Add(this->cbRacquetZone);
			this->groupBox2->Controls->Add(this->label10);
			this->groupBox2->Location = System::Drawing::Point(192, 3);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(188, 102);
			this->groupBox2->TabIndex = 2;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"By Racquet Zone";
			// 
			// cbUnknownZoneCount
			// 
			this->cbUnknownZoneCount->AutoSize = true;
			this->cbUnknownZoneCount->Location = System::Drawing::Point(52, 82);
			this->cbUnknownZoneCount->Name = L"cbUnknownZoneCount";
			this->cbUnknownZoneCount->Size = System::Drawing::Size(123, 17);
			this->cbUnknownZoneCount->TabIndex = 8;
			this->cbUnknownZoneCount->Text = L"Tx Unknown Counts";
			this->cbUnknownZoneCount->UseVisualStyleBackColor = true;
			// 
			// lbZoneStrokes
			// 
			this->lbZoneStrokes->AutoSize = true;
			this->lbZoneStrokes->Location = System::Drawing::Point(80, 64);
			this->lbZoneStrokes->Name = L"lbZoneStrokes";
			this->lbZoneStrokes->Size = System::Drawing::Size(19, 13);
			this->lbZoneStrokes->TabIndex = 7;
			this->lbZoneStrokes->Text = L"----";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(6, 64);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(68, 13);
			this->label12->TabIndex = 6;
			this->label12->Text = L"#  of Strokes";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(9, 40);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(65, 13);
			this->label11->TabIndex = 5;
			this->label11->Text = L"Stroke Type";
			// 
			// cbZoneStrokes
			// 
			this->cbZoneStrokes->FormattingEnabled = true;
			this->cbZoneStrokes->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"All Strokes", L"Other", L"Serve", L"Forehand", 
				L"Backhand", L"Smash", L"Unknown"});
			this->cbZoneStrokes->Location = System::Drawing::Point(80, 37);
			this->cbZoneStrokes->Margin = System::Windows::Forms::Padding(3, 1, 3, 1);
			this->cbZoneStrokes->Name = L"cbZoneStrokes";
			this->cbZoneStrokes->Size = System::Drawing::Size(95, 21);
			this->cbZoneStrokes->TabIndex = 2;
			this->cbZoneStrokes->Text = L"Backhand";
			this->cbZoneStrokes->SelectedIndexChanged += gcnew System::EventHandler(this, &RacquetSensor::cbZoneStrokes_SelectedIndexChanged);
			// 
			// cbRacquetZone
			// 
			this->cbRacquetZone->FormattingEnabled = true;
			this->cbRacquetZone->Items->AddRange(gcnew cli::array< System::Object^  >(12) {L"Miss", L"Top Right", L"Middle Right", L"Bottom Right", 
				L"Bottom Center", L"Bottom Left", L"Middle Center", L"Middle Left", L"Top Left", L"Top Center", L"Rim/Other", L"Unknown"});
			this->cbRacquetZone->Location = System::Drawing::Point(80, 14);
			this->cbRacquetZone->Margin = System::Windows::Forms::Padding(3, 3, 3, 1);
			this->cbRacquetZone->Name = L"cbRacquetZone";
			this->cbRacquetZone->Size = System::Drawing::Size(95, 21);
			this->cbRacquetZone->TabIndex = 1;
			this->cbRacquetZone->Text = L"Bottom Center";
			this->cbRacquetZone->SelectedIndexChanged += gcnew System::EventHandler(this, &RacquetSensor::cbRacquetZone_SelectedIndexChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(42, 17);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(32, 13);
			this->label10->TabIndex = 0;
			this->label10->Text = L"Zone";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->cbUnknownStrokeCount);
			this->groupBox1->Controls->Add(this->lbStrokes);
			this->groupBox1->Controls->Add(this->label9);
			this->groupBox1->Controls->Add(this->cbStrokeType);
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Location = System::Drawing::Point(6, 3);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(180, 102);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"By Stroke";
			// 
			// cbUnknownStrokeCount
			// 
			this->cbUnknownStrokeCount->AutoSize = true;
			this->cbUnknownStrokeCount->Location = System::Drawing::Point(49, 82);
			this->cbUnknownStrokeCount->Name = L"cbUnknownStrokeCount";
			this->cbUnknownStrokeCount->Size = System::Drawing::Size(117, 17);
			this->cbUnknownStrokeCount->TabIndex = 9;
			this->cbUnknownStrokeCount->Text = L"Tx Unkown Counts";
			this->cbUnknownStrokeCount->UseVisualStyleBackColor = true;
			// 
			// lbStrokes
			// 
			this->lbStrokes->AutoSize = true;
			this->lbStrokes->Location = System::Drawing::Point(77, 57);
			this->lbStrokes->Name = L"lbStrokes";
			this->lbStrokes->Size = System::Drawing::Size(19, 13);
			this->lbStrokes->TabIndex = 8;
			this->lbStrokes->Text = L"----";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(6, 57);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(68, 13);
			this->label9->TabIndex = 2;
			this->label9->Text = L"#  of Strokes";
			// 
			// cbStrokeType
			// 
			this->cbStrokeType->FormattingEnabled = true;
			this->cbStrokeType->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"All Strokes", L"Other", L"Serve", L"Forehand", 
				L"Backhand", L"Smash", L"Unknown"});
			this->cbStrokeType->Location = System::Drawing::Point(76, 24);
			this->cbStrokeType->Margin = System::Windows::Forms::Padding(3, 3, 3, 1);
			this->cbStrokeType->Name = L"cbStrokeType";
			this->cbStrokeType->Size = System::Drawing::Size(90, 21);
			this->cbStrokeType->TabIndex = 1;
			this->cbStrokeType->Text = L"Backhand";
			this->cbStrokeType->SelectedIndexChanged += gcnew System::EventHandler(this, &RacquetSensor::cbStrokeType_SelectedIndexChanged);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(5, 27);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(65, 13);
			this->label8->TabIndex = 0;
			this->label8->Text = L"Stroke Type";
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->lbRacquetZone);
			this->tabPage3->Controls->Add(this->label17);
			this->tabPage3->Controls->Add(this->lbEventTime);
			this->tabPage3->Controls->Add(this->lbBallSpeed);
			this->tabPage3->Controls->Add(this->lbStrokeType);
			this->tabPage3->Controls->Add(this->label16);
			this->tabPage3->Controls->Add(this->label15);
			this->tabPage3->Controls->Add(this->label14);
			this->tabPage3->Controls->Add(this->label13);
			this->tabPage3->Controls->Add(this->cbEventCount);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Margin = System::Windows::Forms::Padding(5);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Size = System::Drawing::Size(386, 108);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Previous Events";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// lbRacquetZone
			// 
			this->lbRacquetZone->AutoSize = true;
			this->lbRacquetZone->Location = System::Drawing::Point(231, 9);
			this->lbRacquetZone->Name = L"lbRacquetZone";
			this->lbRacquetZone->Size = System::Drawing::Size(19, 13);
			this->lbRacquetZone->TabIndex = 9;
			this->lbRacquetZone->Text = L"----";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(144, 9);
			this->label17->Margin = System::Windows::Forms::Padding(5);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(79, 13);
			this->label17->TabIndex = 8;
			this->label17->Text = L"Racquet Zone:";
			// 
			// lbEventTime
			// 
			this->lbEventTime->AutoSize = true;
			this->lbEventTime->Location = System::Drawing::Point(231, 78);
			this->lbEventTime->Name = L"lbEventTime";
			this->lbEventTime->Size = System::Drawing::Size(19, 13);
			this->lbEventTime->TabIndex = 7;
			this->lbEventTime->Text = L"----";
			// 
			// lbBallSpeed
			// 
			this->lbBallSpeed->AutoSize = true;
			this->lbBallSpeed->Location = System::Drawing::Point(231, 55);
			this->lbBallSpeed->Name = L"lbBallSpeed";
			this->lbBallSpeed->Size = System::Drawing::Size(19, 13);
			this->lbBallSpeed->TabIndex = 6;
			this->lbBallSpeed->Text = L"----";
			// 
			// lbStrokeType
			// 
			this->lbStrokeType->AutoSize = true;
			this->lbStrokeType->Location = System::Drawing::Point(231, 32);
			this->lbStrokeType->Name = L"lbStrokeType";
			this->lbStrokeType->Size = System::Drawing::Size(19, 13);
			this->lbStrokeType->TabIndex = 5;
			this->lbStrokeType->Text = L"----";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(159, 78);
			this->label16->Margin = System::Windows::Forms::Padding(5);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(64, 13);
			this->label16->TabIndex = 4;
			this->label16->Text = L"Event Time:";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(162, 55);
			this->label15->Margin = System::Windows::Forms::Padding(5);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(61, 13);
			this->label15->TabIndex = 3;
			this->label15->Text = L"Ball Speed:";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(155, 32);
			this->label14->Margin = System::Windows::Forms::Padding(5);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(68, 13);
			this->label14->TabIndex = 2;
			this->label14->Text = L"Stroke Type:";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(3, 9);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(66, 13);
			this->label13->TabIndex = 1;
			this->label13->Text = L"Event Count";
			// 
			// cbEventCount
			// 
			this->cbEventCount->FormattingEnabled = true;
			this->cbEventCount->Location = System::Drawing::Point(75, 6);
			this->cbEventCount->Name = L"cbEventCount";
			this->cbEventCount->Size = System::Drawing::Size(59, 21);
			this->cbEventCount->TabIndex = 0;
			this->cbEventCount->SelectedIndexChanged += gcnew System::EventHandler(this, &RacquetSensor::cbEventCount_SelectedIndexChanged);
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->panel1);
			this->tabPage5->Controls->Add(this->tabControl2);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Size = System::Drawing::Size(386, 108);
			this->tabPage5->TabIndex = 4;
			this->tabPage5->Text = L"Scoring";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->lbUserMarker);
			this->panel1->Controls->Add(this->label24);
			this->panel1->Controls->Add(this->lbOpponentSetCount);
			this->panel1->Controls->Add(this->lbGameModeText);
			this->panel1->Controls->Add(this->lbOpponentSessionsWon);
			this->panel1->Controls->Add(this->lbMySetCount);
			this->panel1->Controls->Add(this->lbOpponentGameCount);
			this->panel1->Controls->Add(this->lbMySessionsWon);
			this->panel1->Controls->Add(this->lbOpponentScore);
			this->panel1->Controls->Add(this->label34);
			this->panel1->Controls->Add(this->label33);
			this->panel1->Controls->Add(this->lbMyScore);
			this->panel1->Controls->Add(this->lbMyGameCount);
			this->panel1->Controls->Add(this->label26);
			this->panel1->Controls->Add(this->label25);
			this->panel1->Controls->Add(this->label7);
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(386, 108);
			this->panel1->TabIndex = 1;
			// 
			// lbUserMarker
			// 
			this->lbUserMarker->Location = System::Drawing::Point(279, 30);
			this->lbUserMarker->Name = L"lbUserMarker";
			this->lbUserMarker->Size = System::Drawing::Size(93, 13);
			this->lbUserMarker->TabIndex = 3;
			this->lbUserMarker->Text = L"----";
			this->lbUserMarker->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label24->Location = System::Drawing::Point(276, 4);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(96, 13);
			this->label24->TabIndex = 2;
			this->label24->Text = L"User Marker Count";
			// 
			// lbOpponentSetCount
			// 
			this->lbOpponentSetCount->Location = System::Drawing::Point(204, 68);
			this->lbOpponentSetCount->Name = L"lbOpponentSetCount";
			this->lbOpponentSetCount->Size = System::Drawing::Size(54, 13);
			this->lbOpponentSetCount->TabIndex = 3;
			this->lbOpponentSetCount->Text = L"----";
			this->lbOpponentSetCount->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lbGameModeText
			// 
			this->lbGameModeText->Location = System::Drawing::Point(-4, 87);
			this->lbGameModeText->Margin = System::Windows::Forms::Padding(3);
			this->lbGameModeText->Name = L"lbGameModeText";
			this->lbGameModeText->Size = System::Drawing::Size(142, 13);
			this->lbGameModeText->TabIndex = 8;
			this->lbGameModeText->Text = L"Session Count (Session)";
			this->lbGameModeText->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// lbOpponentSessionsWon
			// 
			this->lbOpponentSessionsWon->Location = System::Drawing::Point(204, 87);
			this->lbOpponentSessionsWon->Name = L"lbOpponentSessionsWon";
			this->lbOpponentSessionsWon->Size = System::Drawing::Size(54, 13);
			this->lbOpponentSessionsWon->TabIndex = 3;
			this->lbOpponentSessionsWon->Text = L"----";
			this->lbOpponentSessionsWon->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lbMySetCount
			// 
			this->lbMySetCount->Location = System::Drawing::Point(144, 68);
			this->lbMySetCount->Name = L"lbMySetCount";
			this->lbMySetCount->Size = System::Drawing::Size(54, 13);
			this->lbMySetCount->TabIndex = 2;
			this->lbMySetCount->Text = L"----";
			this->lbMySetCount->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lbOpponentGameCount
			// 
			this->lbOpponentGameCount->Location = System::Drawing::Point(204, 49);
			this->lbOpponentGameCount->Name = L"lbOpponentGameCount";
			this->lbOpponentGameCount->Size = System::Drawing::Size(54, 13);
			this->lbOpponentGameCount->TabIndex = 7;
			this->lbOpponentGameCount->Text = L"----";
			this->lbOpponentGameCount->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lbMySessionsWon
			// 
			this->lbMySessionsWon->Location = System::Drawing::Point(144, 87);
			this->lbMySessionsWon->Name = L"lbMySessionsWon";
			this->lbMySessionsWon->Size = System::Drawing::Size(54, 13);
			this->lbMySessionsWon->TabIndex = 2;
			this->lbMySessionsWon->Text = L"----";
			this->lbMySessionsWon->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lbOpponentScore
			// 
			this->lbOpponentScore->Location = System::Drawing::Point(204, 30);
			this->lbOpponentScore->Name = L"lbOpponentScore";
			this->lbOpponentScore->Size = System::Drawing::Size(54, 13);
			this->lbOpponentScore->TabIndex = 7;
			this->lbOpponentScore->Text = L"----";
			this->lbOpponentScore->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label34
			// 
			this->label34->Location = System::Drawing::Point(19, 68);
			this->label34->Margin = System::Windows::Forms::Padding(3);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(119, 13);
			this->label34->TabIndex = 4;
			this->label34->Text = L"Set Count (Lap)";
			this->label34->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label33
			// 
			this->label33->Location = System::Drawing::Point(19, 49);
			this->label33->Margin = System::Windows::Forms::Padding(3);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(119, 13);
			this->label33->TabIndex = 3;
			this->label33->Text = L"Game Count (Length)";
			this->label33->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// lbMyScore
			// 
			this->lbMyScore->Location = System::Drawing::Point(144, 30);
			this->lbMyScore->Name = L"lbMyScore";
			this->lbMyScore->Size = System::Drawing::Size(54, 13);
			this->lbMyScore->TabIndex = 6;
			this->lbMyScore->Text = L"----";
			this->lbMyScore->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lbMyGameCount
			// 
			this->lbMyGameCount->Location = System::Drawing::Point(144, 49);
			this->lbMyGameCount->Name = L"lbMyGameCount";
			this->lbMyGameCount->Size = System::Drawing::Size(54, 13);
			this->lbMyGameCount->TabIndex = 6;
			this->lbMyGameCount->Text = L"----";
			this->lbMyGameCount->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label26
			// 
			this->label26->Location = System::Drawing::Point(19, 30);
			this->label26->Margin = System::Windows::Forms::Padding(3);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(119, 13);
			this->label26->TabIndex = 2;
			this->label26->Text = L"Points (Event)";
			this->label26->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label25->Location = System::Drawing::Point(204, 4);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(54, 13);
			this->label25->TabIndex = 1;
			this->label25->Text = L"Opponent";
			// 
			// label7
			// 
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(144, 4);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(51, 13);
			this->label7->TabIndex = 0;
			this->label7->Text = L"Player";
			this->label7->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// tabControl2
			// 
			this->tabControl2->Location = System::Drawing::Point(0, 0);
			this->tabControl2->Name = L"tabControl2";
			this->tabControl2->SelectedIndex = 0;
			this->tabControl2->Size = System::Drawing::Size(390, 112);
			this->tabControl2->TabIndex = 0;
			// 
			// tabPage10
			// 
			this->tabPage10->Controls->Add(this->lbTotCaloriesDisplay);
			this->tabPage10->Controls->Add(this->label37);
			this->tabPage10->Controls->Add(this->lbTotDistanceDisplay);
			this->tabPage10->Controls->Add(this->label36);
			this->tabPage10->Controls->Add(this->lbAvgRunSpdDisplay);
			this->tabPage10->Controls->Add(this->lbAvgHRDisplay);
			this->tabPage10->Controls->Add(this->label45);
			this->tabPage10->Controls->Add(this->lbMaxHRDisplay);
			this->tabPage10->Controls->Add(this->lbMaxRunSpdDisplay);
			this->tabPage10->Controls->Add(this->label48);
			this->tabPage10->Controls->Add(this->label49);
			this->tabPage10->Controls->Add(this->label50);
			this->tabPage10->Location = System::Drawing::Point(4, 22);
			this->tabPage10->Name = L"tabPage10";
			this->tabPage10->Size = System::Drawing::Size(386, 108);
			this->tabPage10->TabIndex = 5;
			this->tabPage10->Text = L"Stats";
			this->tabPage10->UseVisualStyleBackColor = true;
			// 
			// lbTotCaloriesDisplay
			// 
			this->lbTotCaloriesDisplay->Location = System::Drawing::Point(277, 87);
			this->lbTotCaloriesDisplay->Name = L"lbTotCaloriesDisplay";
			this->lbTotCaloriesDisplay->Size = System::Drawing::Size(93, 13);
			this->lbTotCaloriesDisplay->TabIndex = 25;
			this->lbTotCaloriesDisplay->Text = L"----";
			this->lbTotCaloriesDisplay->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label37->Location = System::Drawing::Point(285, 61);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(71, 13);
			this->label37->TabIndex = 24;
			this->label37->Text = L"Total Calories";
			// 
			// lbTotDistanceDisplay
			// 
			this->lbTotDistanceDisplay->Location = System::Drawing::Point(277, 42);
			this->lbTotDistanceDisplay->Name = L"lbTotDistanceDisplay";
			this->lbTotDistanceDisplay->Size = System::Drawing::Size(93, 13);
			this->lbTotDistanceDisplay->TabIndex = 17;
			this->lbTotDistanceDisplay->Text = L"----";
			this->lbTotDistanceDisplay->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label36->Location = System::Drawing::Point(285, 16);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(76, 13);
			this->label36->TabIndex = 14;
			this->label36->Text = L"Total Distance";
			// 
			// lbAvgRunSpdDisplay
			// 
			this->lbAvgRunSpdDisplay->Location = System::Drawing::Point(202, 61);
			this->lbAvgRunSpdDisplay->Name = L"lbAvgRunSpdDisplay";
			this->lbAvgRunSpdDisplay->Size = System::Drawing::Size(54, 13);
			this->lbAvgRunSpdDisplay->TabIndex = 22;
			this->lbAvgRunSpdDisplay->Text = L"----";
			this->lbAvgRunSpdDisplay->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lbAvgHRDisplay
			// 
			this->lbAvgHRDisplay->Location = System::Drawing::Point(202, 42);
			this->lbAvgHRDisplay->Name = L"lbAvgHRDisplay";
			this->lbAvgHRDisplay->Size = System::Drawing::Size(54, 13);
			this->lbAvgHRDisplay->TabIndex = 23;
			this->lbAvgHRDisplay->Text = L"----";
			this->lbAvgHRDisplay->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label45
			// 
			this->label45->Location = System::Drawing::Point(17, 61);
			this->label45->Margin = System::Windows::Forms::Padding(3);
			this->label45->Name = L"label45";
			this->label45->Size = System::Drawing::Size(119, 13);
			this->label45->TabIndex = 15;
			this->label45->Text = L"Running Speed (mm/s)";
			this->label45->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// lbMaxHRDisplay
			// 
			this->lbMaxHRDisplay->Location = System::Drawing::Point(142, 42);
			this->lbMaxHRDisplay->Name = L"lbMaxHRDisplay";
			this->lbMaxHRDisplay->Size = System::Drawing::Size(54, 13);
			this->lbMaxHRDisplay->TabIndex = 21;
			this->lbMaxHRDisplay->Text = L"----";
			this->lbMaxHRDisplay->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lbMaxRunSpdDisplay
			// 
			this->lbMaxRunSpdDisplay->Location = System::Drawing::Point(142, 61);
			this->lbMaxRunSpdDisplay->Name = L"lbMaxRunSpdDisplay";
			this->lbMaxRunSpdDisplay->Size = System::Drawing::Size(54, 13);
			this->lbMaxRunSpdDisplay->TabIndex = 20;
			this->lbMaxRunSpdDisplay->Text = L"----";
			this->lbMaxRunSpdDisplay->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label48
			// 
			this->label48->Location = System::Drawing::Point(17, 42);
			this->label48->Margin = System::Windows::Forms::Padding(3);
			this->label48->Name = L"label48";
			this->label48->Size = System::Drawing::Size(119, 13);
			this->label48->TabIndex = 13;
			this->label48->Text = L"Heart Rate (bpm)";
			this->label48->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label49
			// 
			this->label49->AutoSize = true;
			this->label49->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label49->Location = System::Drawing::Point(202, 16);
			this->label49->Name = L"label49";
			this->label49->Size = System::Drawing::Size(47, 13);
			this->label49->TabIndex = 10;
			this->label49->Text = L"Average";
			// 
			// label50
			// 
			this->label50->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Italic | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label50->Location = System::Drawing::Point(142, 16);
			this->label50->Name = L"label50";
			this->label50->Size = System::Drawing::Size(51, 13);
			this->label50->TabIndex = 9;
			this->label50->Text = L"Maximum";
			this->label50->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->tabControl3);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Size = System::Drawing::Size(386, 108);
			this->tabPage4->TabIndex = 3;
			this->tabPage4->Text = L"Common Data";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// tabControl3
			// 
			this->tabControl3->Controls->Add(this->tabPage6);
			this->tabControl3->Controls->Add(this->tabPage7);
			this->tabControl3->Location = System::Drawing::Point(0, 3);
			this->tabControl3->Name = L"tabControl3";
			this->tabControl3->SelectedIndex = 0;
			this->tabControl3->Size = System::Drawing::Size(390, 105);
			this->tabControl3->TabIndex = 0;
			// 
			// tabPage6
			// 
			this->tabPage6->Controls->Add(this->cbInvalidSerial);
			this->tabPage6->Controls->Add(this->btUpdateCommon);
			this->tabPage6->Controls->Add(this->lbCommonError);
			this->tabPage6->Controls->Add(this->label_Glb_SerialNum);
			this->tabPage6->Controls->Add(this->tbManfId);
			this->tabPage6->Controls->Add(this->label_Glb_ManfID);
			this->tabPage6->Controls->Add(this->tbHardwareVer);
			this->tabPage6->Controls->Add(this->label_Glb_ModelNum);
			this->tabPage6->Controls->Add(this->tbSoftwareVer);
			this->tabPage6->Controls->Add(this->tbModelNum);
			this->tabPage6->Controls->Add(this->label_Glb_HardwareVer);
			this->tabPage6->Controls->Add(this->tbSerialNumber);
			this->tabPage6->Controls->Add(this->label_Glb_SoftwareVer);
			this->tabPage6->Location = System::Drawing::Point(4, 22);
			this->tabPage6->Name = L"tabPage6";
			this->tabPage6->Padding = System::Windows::Forms::Padding(3);
			this->tabPage6->Size = System::Drawing::Size(382, 79);
			this->tabPage6->TabIndex = 0;
			this->tabPage6->Text = L"Product/Manf. Info";
			this->tabPage6->UseVisualStyleBackColor = true;
			// 
			// cbInvalidSerial
			// 
			this->cbInvalidSerial->AutoSize = true;
			this->cbInvalidSerial->Location = System::Drawing::Point(140, 5);
			this->cbInvalidSerial->Name = L"cbInvalidSerial";
			this->cbInvalidSerial->Size = System::Drawing::Size(79, 17);
			this->cbInvalidSerial->TabIndex = 76;
			this->cbInvalidSerial->Text = L"No Serial #";
			this->cbInvalidSerial->UseVisualStyleBackColor = true;
			this->cbInvalidSerial->CheckedChanged += gcnew System::EventHandler(this, &RacquetSensor::cbInvalidSerial_CheckedChanged);
			// 
			// btUpdateCommon
			// 
			this->btUpdateCommon->Location = System::Drawing::Point(259, 55);
			this->btUpdateCommon->Name = L"btUpdateCommon";
			this->btUpdateCommon->Size = System::Drawing::Size(97, 20);
			this->btUpdateCommon->TabIndex = 75;
			this->btUpdateCommon->Text = L"Update All";
			this->btUpdateCommon->UseVisualStyleBackColor = true;
			this->btUpdateCommon->Click += gcnew System::EventHandler(this, &RacquetSensor::btUpdateCommon_Click);
			// 
			// lbCommonError
			// 
			this->lbCommonError->AutoSize = true;
			this->lbCommonError->ForeColor = System::Drawing::Color::Red;
			this->lbCommonError->Location = System::Drawing::Point(161, 59);
			this->lbCommonError->Name = L"lbCommonError";
			this->lbCommonError->Size = System::Drawing::Size(92, 13);
			this->lbCommonError->TabIndex = 69;
			this->lbCommonError->Text = L"Error: Invalid input\r\n";
			this->lbCommonError->Visible = false;
			// 
			// label_Glb_SerialNum
			// 
			this->label_Glb_SerialNum->AutoSize = true;
			this->label_Glb_SerialNum->Location = System::Drawing::Point(6, 6);
			this->label_Glb_SerialNum->Name = L"label_Glb_SerialNum";
			this->label_Glb_SerialNum->Size = System::Drawing::Size(46, 13);
			this->label_Glb_SerialNum->TabIndex = 65;
			this->label_Glb_SerialNum->Text = L"Serial #:";
			// 
			// tbManfId
			// 
			this->tbManfId->Location = System::Drawing::Point(85, 29);
			this->tbManfId->MaxLength = 5;
			this->tbManfId->Name = L"tbManfId";
			this->tbManfId->Size = System::Drawing::Size(49, 20);
			this->tbManfId->TabIndex = 70;
			this->tbManfId->Text = L"2";
			// 
			// label_Glb_ManfID
			// 
			this->label_Glb_ManfID->AutoSize = true;
			this->label_Glb_ManfID->Location = System::Drawing::Point(1, 32);
			this->label_Glb_ManfID->Name = L"label_Glb_ManfID";
			this->label_Glb_ManfID->Size = System::Drawing::Size(51, 13);
			this->label_Glb_ManfID->TabIndex = 64;
			this->label_Glb_ManfID->Text = L"Manf. ID:";
			// 
			// tbHardwareVer
			// 
			this->tbHardwareVer->Location = System::Drawing::Point(327, 3);
			this->tbHardwareVer->MaxLength = 3;
			this->tbHardwareVer->Name = L"tbHardwareVer";
			this->tbHardwareVer->Size = System::Drawing::Size(29, 20);
			this->tbHardwareVer->TabIndex = 72;
			this->tbHardwareVer->Text = L"1";
			// 
			// label_Glb_ModelNum
			// 
			this->label_Glb_ModelNum->AutoSize = true;
			this->label_Glb_ModelNum->Location = System::Drawing::Point(3, 60);
			this->label_Glb_ModelNum->Name = L"label_Glb_ModelNum";
			this->label_Glb_ModelNum->Size = System::Drawing::Size(49, 13);
			this->label_Glb_ModelNum->TabIndex = 68;
			this->label_Glb_ModelNum->Text = L"Model #:";
			// 
			// tbSoftwareVer
			// 
			this->tbSoftwareVer->Location = System::Drawing::Point(327, 32);
			this->tbSoftwareVer->MaxLength = 3;
			this->tbSoftwareVer->Name = L"tbSoftwareVer";
			this->tbSoftwareVer->Size = System::Drawing::Size(29, 20);
			this->tbSoftwareVer->TabIndex = 74;
			this->tbSoftwareVer->Text = L"1";
			// 
			// tbModelNum
			// 
			this->tbModelNum->Location = System::Drawing::Point(85, 57);
			this->tbModelNum->MaxLength = 5;
			this->tbModelNum->Name = L"tbModelNum";
			this->tbModelNum->Size = System::Drawing::Size(49, 20);
			this->tbModelNum->TabIndex = 71;
			this->tbModelNum->Text = L"33669";
			// 
			// label_Glb_HardwareVer
			// 
			this->label_Glb_HardwareVer->AutoSize = true;
			this->label_Glb_HardwareVer->Location = System::Drawing::Point(246, 6);
			this->label_Glb_HardwareVer->Name = L"label_Glb_HardwareVer";
			this->label_Glb_HardwareVer->Size = System::Drawing::Size(75, 13);
			this->label_Glb_HardwareVer->TabIndex = 66;
			this->label_Glb_HardwareVer->Text = L"Hardware Ver:";
			// 
			// tbSerialNumber
			// 
			this->tbSerialNumber->Location = System::Drawing::Point(58, 3);
			this->tbSerialNumber->MaxLength = 10;
			this->tbSerialNumber->Name = L"tbSerialNumber";
			this->tbSerialNumber->Size = System::Drawing::Size(76, 20);
			this->tbSerialNumber->TabIndex = 73;
			this->tbSerialNumber->Text = L"1234567890";
			// 
			// label_Glb_SoftwareVer
			// 
			this->label_Glb_SoftwareVer->AutoSize = true;
			this->label_Glb_SoftwareVer->Location = System::Drawing::Point(250, 32);
			this->label_Glb_SoftwareVer->Name = L"label_Glb_SoftwareVer";
			this->label_Glb_SoftwareVer->Size = System::Drawing::Size(71, 13);
			this->label_Glb_SoftwareVer->TabIndex = 67;
			this->label_Glb_SoftwareVer->Text = L"Software Ver:";
			// 
			// tabPage7
			// 
			this->tabPage7->Controls->Add(this->label35);
			this->tabPage7->Controls->Add(this->radioResolution2);
			this->tabPage7->Controls->Add(this->comboBoxBatStatus);
			this->tabPage7->Controls->Add(this->labelTimeError);
			this->tabPage7->Controls->Add(this->label28);
			this->tabPage7->Controls->Add(this->radioResolution16);
			this->tabPage7->Controls->Add(this->buttonUpdateTime);
			this->tabPage7->Controls->Add(this->textBoxTime);
			this->tabPage7->Controls->Add(this->labelElpTime);
			this->tabPage7->Controls->Add(this->label_Bat_ElpTime);
			this->tabPage7->Controls->Add(this->label30);
			this->tabPage7->Controls->Add(this->label29);
			this->tabPage7->Controls->Add(this->numericCoarseVoltage);
			this->tabPage7->Controls->Add(this->numericFractionalVoltage);
			this->tabPage7->Controls->Add(this->checkBox_Bat_Voltage);
			this->tabPage7->Location = System::Drawing::Point(4, 22);
			this->tabPage7->Name = L"tabPage7";
			this->tabPage7->Padding = System::Windows::Forms::Padding(3);
			this->tabPage7->Size = System::Drawing::Size(382, 79);
			this->tabPage7->TabIndex = 1;
			this->tabPage7->Text = L"Battery Status";
			this->tabPage7->UseVisualStyleBackColor = true;
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(222, 30);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(74, 13);
			this->label35->TabIndex = 92;
			this->label35->Text = L"Resolution (s):";
			// 
			// radioResolution2
			// 
			this->radioResolution2->AutoSize = true;
			this->radioResolution2->Checked = true;
			this->radioResolution2->Location = System::Drawing::Point(302, 28);
			this->radioResolution2->Name = L"radioResolution2";
			this->radioResolution2->Size = System::Drawing::Size(31, 17);
			this->radioResolution2->TabIndex = 58;
			this->radioResolution2->TabStop = true;
			this->radioResolution2->Text = L"2";
			this->radioResolution2->UseVisualStyleBackColor = true;
			this->radioResolution2->CheckedChanged += gcnew System::EventHandler(this, &RacquetSensor::radioResolution2_CheckedChanged);
			// 
			// comboBoxBatStatus
			// 
			this->comboBoxBatStatus->FormattingEnabled = true;
			this->comboBoxBatStatus->Items->AddRange(gcnew cli::array< System::Object^  >(6) {L"New", L"Good", L"Ok", L"Low", L"Critical", 
				L"Invalid"});
			this->comboBoxBatStatus->Location = System::Drawing::Point(110, 30);
			this->comboBoxBatStatus->Name = L"comboBoxBatStatus";
			this->comboBoxBatStatus->Size = System::Drawing::Size(60, 21);
			this->comboBoxBatStatus->TabIndex = 80;
			this->comboBoxBatStatus->Text = L"Ok";
			this->comboBoxBatStatus->SelectedIndexChanged += gcnew System::EventHandler(this, &RacquetSensor::comboBoxBatStatus_SelectedIndexChanged);
			// 
			// labelTimeError
			// 
			this->labelTimeError->AutoSize = true;
			this->labelTimeError->ForeColor = System::Drawing::Color::Red;
			this->labelTimeError->Location = System::Drawing::Point(185, 58);
			this->labelTimeError->Name = L"labelTimeError";
			this->labelTimeError->Size = System::Drawing::Size(32, 13);
			this->labelTimeError->TabIndex = 91;
			this->labelTimeError->Text = L"Error!";
			this->labelTimeError->Visible = false;
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(28, 30);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(76, 13);
			this->label28->TabIndex = 81;
			this->label28->Text = L"Battery Status:";
			// 
			// radioResolution16
			// 
			this->radioResolution16->AutoSize = true;
			this->radioResolution16->Location = System::Drawing::Point(339, 28);
			this->radioResolution16->Name = L"radioResolution16";
			this->radioResolution16->Size = System::Drawing::Size(37, 17);
			this->radioResolution16->TabIndex = 59;
			this->radioResolution16->Text = L"16";
			this->radioResolution16->UseVisualStyleBackColor = true;
			// 
			// buttonUpdateTime
			// 
			this->buttonUpdateTime->Location = System::Drawing::Point(223, 53);
			this->buttonUpdateTime->Name = L"buttonUpdateTime";
			this->buttonUpdateTime->Size = System::Drawing::Size(84, 20);
			this->buttonUpdateTime->TabIndex = 90;
			this->buttonUpdateTime->Text = L"Update Time";
			this->buttonUpdateTime->UseVisualStyleBackColor = true;
			this->buttonUpdateTime->Click += gcnew System::EventHandler(this, &RacquetSensor::buttonUpdateTime_Click);
			// 
			// textBoxTime
			// 
			this->textBoxTime->Location = System::Drawing::Point(313, 53);
			this->textBoxTime->MaxLength = 9;
			this->textBoxTime->Name = L"textBoxTime";
			this->textBoxTime->Size = System::Drawing::Size(63, 20);
			this->textBoxTime->TabIndex = 89;
			this->textBoxTime->Text = L"0";
			this->textBoxTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// labelElpTime
			// 
			this->labelElpTime->Location = System::Drawing::Point(315, 4);
			this->labelElpTime->Name = L"labelElpTime";
			this->labelElpTime->Size = System::Drawing::Size(62, 13);
			this->labelElpTime->TabIndex = 87;
			this->labelElpTime->Text = L"0";
			this->labelElpTime->TextAlign = System::Drawing::ContentAlignment::BottomRight;
			// 
			// label_Bat_ElpTime
			// 
			this->label_Bat_ElpTime->AutoSize = true;
			this->label_Bat_ElpTime->Location = System::Drawing::Point(221, 3);
			this->label_Bat_ElpTime->Name = L"label_Bat_ElpTime";
			this->label_Bat_ElpTime->Size = System::Drawing::Size(88, 13);
			this->label_Bat_ElpTime->TabIndex = 86;
			this->label_Bat_ElpTime->Text = L"Elapsed Time (s):";
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(6, 4);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(98, 13);
			this->label30->TabIndex = 2;
			this->label30->Text = L"Coarse Voltage (V):";
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(-1, 58);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(121, 13);
			this->label29->TabIndex = 85;
			this->label29->Text = L"Frac. Voltage (1/256 V):";
			// 
			// numericCoarseVoltage
			// 
			this->numericCoarseVoltage->Location = System::Drawing::Point(110, 4);
			this->numericCoarseVoltage->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {14, 0, 0, 0});
			this->numericCoarseVoltage->Name = L"numericCoarseVoltage";
			this->numericCoarseVoltage->Size = System::Drawing::Size(42, 20);
			this->numericCoarseVoltage->TabIndex = 82;
			this->numericCoarseVoltage->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			this->numericCoarseVoltage->ValueChanged += gcnew System::EventHandler(this, &RacquetSensor::numericCoarseVoltage_ValueChanged);
			// 
			// numericFractionalVoltage
			// 
			this->numericFractionalVoltage->Location = System::Drawing::Point(128, 56);
			this->numericFractionalVoltage->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->numericFractionalVoltage->Name = L"numericFractionalVoltage";
			this->numericFractionalVoltage->Size = System::Drawing::Size(42, 20);
			this->numericFractionalVoltage->TabIndex = 83;
			this->numericFractionalVoltage->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {85, 0, 0, 0});
			this->numericFractionalVoltage->ValueChanged += gcnew System::EventHandler(this, &RacquetSensor::numericFractionalVoltage_ValueChanged);
			// 
			// checkBox_Bat_Voltage
			// 
			this->checkBox_Bat_Voltage->AutoSize = true;
			this->checkBox_Bat_Voltage->Location = System::Drawing::Point(158, 4);
			this->checkBox_Bat_Voltage->Name = L"checkBox_Bat_Voltage";
			this->checkBox_Bat_Voltage->Size = System::Drawing::Size(57, 17);
			this->checkBox_Bat_Voltage->TabIndex = 84;
			this->checkBox_Bat_Voltage->Text = L"Invalid";
			this->checkBox_Bat_Voltage->UseVisualStyleBackColor = true;
			this->checkBox_Bat_Voltage->CheckedChanged += gcnew System::EventHandler(this, &RacquetSensor::checkBox_Bat_Voltage_CheckedChanged);
			// 
			// lbGameMode
			// 
			this->lbGameMode->AutoSize = true;
			this->lbGameMode->Location = System::Drawing::Point(130, 72);
			this->lbGameMode->Name = L"lbGameMode";
			this->lbGameMode->Size = System::Drawing::Size(19, 13);
			this->lbGameMode->TabIndex = 1;
			this->lbGameMode->Text = L"----";
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(56, 72);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(68, 13);
			this->label27->TabIndex = 0;
			this->label27->Text = L"Game Mode:";
			// 
			// panel_Display
			// 
			this->panel_Display->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel_Display->Controls->Add(this->lbSessionTime);
			this->panel_Display->Controls->Add(this->label32);
			this->panel_Display->Controls->Add(this->lbGameMode);
			this->panel_Display->Controls->Add(this->lbDisplayStroke);
			this->panel_Display->Controls->Add(this->label27);
			this->panel_Display->Controls->Add(this->lbDisplaySpeed);
			this->panel_Display->Controls->Add(this->lbDisplayEvent);
			this->panel_Display->Controls->Add(this->lbDisplayTime);
			this->panel_Display->Controls->Add(this->label19);
			this->panel_Display->Controls->Add(this->label21);
			this->panel_Display->Controls->Add(this->lbDisplayZone);
			this->panel_Display->Controls->Add(this->label23);
			this->panel_Display->Controls->Add(this->label22);
			this->panel_Display->Controls->Add(this->label20);
			this->panel_Display->Controls->Add(this->label18);
			this->panel_Display->Location = System::Drawing::Point(45, 182);
			this->panel_Display->Name = L"panel_Display";
			this->panel_Display->Size = System::Drawing::Size(222, 105);
			this->panel_Display->TabIndex = 1;
			// 
			// lbSessionTime
			// 
			this->lbSessionTime->AutoSize = true;
			this->lbSessionTime->Location = System::Drawing::Point(130, 88);
			this->lbSessionTime->Name = L"lbSessionTime";
			this->lbSessionTime->Size = System::Drawing::Size(49, 13);
			this->lbSessionTime->TabIndex = 11;
			this->lbSessionTime->Text = L"00:00:00";
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(62, 88);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(62, 13);
			this->label32->TabIndex = 10;
			this->label32->Text = L"Timer Time:";
			// 
			// lbDisplayStroke
			// 
			this->lbDisplayStroke->AutoSize = true;
			this->lbDisplayStroke->Location = System::Drawing::Point(44, 37);
			this->lbDisplayStroke->Name = L"lbDisplayStroke";
			this->lbDisplayStroke->Size = System::Drawing::Size(19, 13);
			this->lbDisplayStroke->TabIndex = 9;
			this->lbDisplayStroke->Text = L"----";
			// 
			// lbDisplaySpeed
			// 
			this->lbDisplaySpeed->AutoSize = true;
			this->lbDisplaySpeed->Location = System::Drawing::Point(44, 53);
			this->lbDisplaySpeed->Name = L"lbDisplaySpeed";
			this->lbDisplaySpeed->Size = System::Drawing::Size(19, 13);
			this->lbDisplaySpeed->TabIndex = 8;
			this->lbDisplaySpeed->Text = L"----";
			// 
			// lbDisplayEvent
			// 
			this->lbDisplayEvent->AutoSize = true;
			this->lbDisplayEvent->Location = System::Drawing::Point(156, 37);
			this->lbDisplayEvent->Name = L"lbDisplayEvent";
			this->lbDisplayEvent->Size = System::Drawing::Size(19, 13);
			this->lbDisplayEvent->TabIndex = 5;
			this->lbDisplayEvent->Text = L"----";
			// 
			// lbDisplayTime
			// 
			this->lbDisplayTime->AutoSize = true;
			this->lbDisplayTime->Location = System::Drawing::Point(156, 21);
			this->lbDisplayTime->Name = L"lbDisplayTime";
			this->lbDisplayTime->Size = System::Drawing::Size(19, 13);
			this->lbDisplayTime->TabIndex = 6;
			this->lbDisplayTime->Text = L"----";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(109, 37);
			this->label19->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(48, 13);
			this->label19->TabIndex = 1;
			this->label19->Text = L"Event #:";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(124, 21);
			this->label21->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(33, 13);
			this->label21->TabIndex = 3;
			this->label21->Text = L"Time:";
			// 
			// lbDisplayZone
			// 
			this->lbDisplayZone->AutoSize = true;
			this->lbDisplayZone->Location = System::Drawing::Point(44, 21);
			this->lbDisplayZone->Name = L"lbDisplayZone";
			this->lbDisplayZone->Size = System::Drawing::Size(19, 13);
			this->lbDisplayZone->TabIndex = 7;
			this->lbDisplayZone->Text = L"----";
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(3, 53);
			this->label23->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(41, 13);
			this->label23->TabIndex = 3;
			this->label23->Text = L"Speed:";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(3, 37);
			this->label22->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(41, 13);
			this->label22->TabIndex = 4;
			this->label22->Text = L"Stroke:";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(9, 21);
			this->label20->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(35, 13);
			this->label20->TabIndex = 2;
			this->label20->Text = L"Zone:";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label18->Location = System::Drawing::Point(3, 2);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(129, 13);
			this->label18->TabIndex = 0;
			this->label18->Text = L"Current Racquet Tx Data:";
			// 
			// checkANTFS
			// 
			this->checkANTFS->AutoSize = true;
			this->checkANTFS->Location = System::Drawing::Point(278, 88);
			this->checkANTFS->Name = L"checkANTFS";
			this->checkANTFS->Size = System::Drawing::Size(104, 17);
			this->checkANTFS->TabIndex = 29;
			this->checkANTFS->Text = L"ANT-FS Support";
			this->checkANTFS->UseVisualStyleBackColor = true;
			this->checkANTFS->CheckedChanged += gcnew System::EventHandler(this, &RacquetSensor::checkANTFS_CheckedChanged);
			// 
			// checkKMpH
			// 
			this->checkKMpH->AutoSize = true;
			this->checkKMpH->Location = System::Drawing::Point(294, 70);
			this->checkKMpH->Name = L"checkKMpH";
			this->checkKMpH->Size = System::Drawing::Size(50, 17);
			this->checkKMpH->TabIndex = 26;
			this->checkKMpH->TabStop = true;
			this->checkKMpH->Text = L"km/h";
			this->checkKMpH->UseVisualStyleBackColor = true;
			// 
			// RacquetSensor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(794, 351);
			this->Controls->Add(this->panel_Display);
			this->Controls->Add(this->panel_Settings);
			this->Name = L"RacquetSensor";
			this->Text = L"Racquet Sensor Simulator";
			this->panel_Settings->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->valueSpeed))->EndInit();
			this->tabPage2->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			this->tabPage5->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->tabPage10->ResumeLayout(false);
			this->tabPage10->PerformLayout();
			this->tabPage4->ResumeLayout(false);
			this->tabControl3->ResumeLayout(false);
			this->tabPage6->ResumeLayout(false);
			this->tabPage6->PerformLayout();
			this->tabPage7->ResumeLayout(false);
			this->tabPage7->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericCoarseVoltage))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericFractionalVoltage))->EndInit();
			this->panel_Display->ResumeLayout(false);
			this->panel_Display->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion



};