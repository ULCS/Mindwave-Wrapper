// test mind wave.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "thinkgear.h"


void wait() {
	printf( "\n" );
	printf( "Press the ENTER key...\n" );
	fflush( stdout );
	getc( stdin );
}

class Mindwave
{

private:
	// Connection details
	char *_McomPort; 
	int   _MdllVersion;
	int   _MconnectionId;
	int   _MpacketsRead0;
	int   _MerrCode;
	int   _Mtemp;
	FILE *_MdataFile; // used for neural networks

	/////////////////////////////////////////////////
	// Variables
	int errCode;	// Code returned from mindwave

	int _MAttention; 
	int _MMeditation; 
	int _MBlink;

	int _MpacketsRead; 

	//////////////////////////////////////////
	// Prototypes


public:
	void connect();
	void disconnect(); 

	void updateAttention();
	void updateMeditation(); 
	void updateBlink();

	int getAttention()
	{

		return _MAttention;
	}
	int getMeditation()
	{

		return _MMeditation;
	}
	int getBlink()
	{
		int temp = _MBlink;
		_MBlink = 0; 
		return temp; 
	}


	Mindwave();
};
Mindwave::Mindwave()
{
	char *_McomPort = NULL;
	int   _MdllVersion = 0;
	int   _MconnectionId = 0;
	int   _MpacketsRead = 0;
	int   errCode = 0;
	int   _Mtemp = 0;
}

void Mindwave::connect()
{
	_MdataFile = fopen("dataStore.txt", "a+");

	/* Print driver version number */
	_MdllVersion = TG_GetDriverVersion();
	printf( "ThinkGear DLL version: %d\n", _MdllVersion );

	/* Get a connection ID handle to ThinkGear */
	_MconnectionId = TG_GetNewConnectionId();
	if( _MconnectionId < 0 ) {
		fprintf( stderr, "ERROR: TG_GetNewConnectionId() returned %d.\n", 
			_MconnectionId );
		wait();
		exit( EXIT_FAILURE );
	}

	/* Attempt to connect the connection ID handle to serial port "COM5" */
	_McomPort = "\\\\.\\COM28";
	errCode = TG_Connect( _MconnectionId, 
		_McomPort, 
		TG_BAUD_57600, 
		TG_STREAM_PACKETS );
	if( errCode < 0 ) {
		fprintf( stderr, "ERROR: TG_Connect() returned %d.\n", errCode );
		wait();
		exit( EXIT_FAILURE );
	}

	errCode = TG_EnableBlinkDetection( _MconnectionId, 1 );
	if( errCode < 0 ) {
		fprintf( stderr, "ERROR: TG_EnableBlinkDetection() returned %d.\n", errCode );
		wait();
		exit( EXIT_FAILURE );
	}

}

void Mindwave::disconnect()
{
	TG_FreeConnection( _MconnectionId );

	/* End program */
	printf(" - Closing Program");
	wait();

}

void Mindwave::updateAttention()
{
	_Mtemp = 0; 
	//_MpacketsRead = 0; 
	while(_MpacketsRead < 1000)
	{
		/* Attempt to read a Packet of data from the connection */
		errCode = TG_ReadPackets( _MconnectionId, 1 );
		/* If TG_ReadPackets() was able to read a complete Packet of data... */
		if( errCode != -1 && errCode != -2  ) 
		{
			/* If attention value has been updated by TG_ReadPackets()... */
			if( TG_GetValueStatus(_MconnectionId, TG_DATA_ATTENTION) != 0 ) 
			{
				/* Get and print out the updated attention value */
				_Mtemp = (int)TG_GetValue(_MconnectionId, TG_DATA_ATTENTION);


				if (_Mtemp > 0) {_MAttention = _Mtemp;
				//printf("%d", _MAttention);
				break;
				}

			}
			/* end "If attention value has been updated..." */
		}
	}
}

void Mindwave::updateMeditation()
{
	_Mtemp = 0; 
	//_MpacketsRead = 0; 
	while(_MpacketsRead < 1000)
	{
		/* Attempt to read a Packet of data from the connection */
		errCode = TG_ReadPackets( _MconnectionId, 1 );
		/* If TG_ReadPackets() was able to read a complete Packet of data... */
		if( errCode != -1 && errCode != -2  ) 
		{
			/* If attention value has been updated by TG_ReadPackets()... */
			if( TG_GetValueStatus(_MconnectionId, TG_DATA_MEDITATION) != 0 ) 
			{
				/* Get and print out the updated attention value */
				_Mtemp = (int)TG_GetValue(_MconnectionId, TG_DATA_MEDITATION);


				if (_Mtemp > 0) {_MMeditation = _Mtemp;
				//printf("%d", _MAttention);
				break;
				}

			}
			/* end "If attention value has been updated..." */
		}
	}
}

void Mindwave::updateBlink()
{
	_Mtemp = 0; 
	//_MpacketsRead = 0; 
	while(_MpacketsRead < 1000)
	{
		/* Attempt to read a Packet of data from the connection */
		errCode = TG_ReadPackets( _MconnectionId, 1 );
		/* If TG_ReadPackets() was able to read a complete Packet of data... */
		if( errCode !=-2 && errCode !=-1 ) 
		{
			/* If attention value has been updated by TG_ReadPackets()... */
			if( TG_GetValueStatus(_MconnectionId, TG_DATA_BLINK_STRENGTH) != 0 ) 
			{
				/* Get and print out the updated attention value */
				_Mtemp = (int)TG_GetValue(_MconnectionId, TG_DATA_BLINK_STRENGTH);


				if (_Mtemp > 0) {_MBlink = _Mtemp;
				//printf("%d", _MAttention);
				break;
				}

			}
		}
		else if (errCode == -1)
		{
			// no connection
		}
		else if(errCode == -2)
		{
			//use previous data
			if (_Mtemp > 0) {_MBlink = _Mtemp;
			//printf("%d", _MAttention);
			break;
			}


			/* end "If attention value has been updated..." */
		}
	}
}






//void Mindwave::setMeditation(int value)
//{
//	 
//	while(_MpacketsRead < 1000)
//	{
//		/* Attempt to read a Packet of data from the connection */
//		errCode = TG_ReadPackets( _MconnectionId, 1 );
//		/* If TG_ReadPackets() was able to read a complete Packet of data... */
//		if( errCode == 1 ) 
//		{
//			if( TG_GetValueStatus(_MconnectionId, TG_DATA_MEDITATION) != 0 ) 
//			{
//				/* Get and print out the updated attention value */
//				_Mtemp = (int)TG_GetValue(_MconnectionId, TG_DATA_MEDITATION);
//
//				//fprintf( _MdataFile, "MED Value: %d\n", _Mtemp);
//				//fflush( _MdataFile );
//				return _Mtemp;
//			} /* end "If attention value has been updated..." */
//			
//		}
//	}
//	return _Mtemp;
//}
//
//void Mindwave::setBlink(int value)
//{
//	
//	while(_MpacketsRead < 10)
//	{
//		/* Attempt to read a Packet of data from the connection */
//		errCode = TG_ReadPackets( _MconnectionId, 1 );
//		/* If TG_ReadPackets() was able to read a complete Packet of data... */
//		if( errCode != -1 && errCode != -2 ) 
//		{
//				/* If attention value has been updated by TG_ReadPackets()... */
//				if( TG_GetValueStatus(_MconnectionId, TG_DATA_BLINK_STRENGTH) != 0 ) 
//				{
//					/* Get and print out the updated attention value */
//					_Mtemp = (int)TG_GetValue(_MconnectionId, TG_DATA_BLINK_STRENGTH);
//					//fprintf( _MdataFile, "BLINK Value: %d\n", _Mtemp);
//					//fflush( _MdataFile );
//					return _Mtemp;
//				} /* end "If attention value has been updated..." */
//		
//		}
//		else if( errCode == -1)
//		{
//			//return _Mtemp;
//			//no connection
//		}
//		else if (errCode ==-2)
//		{
//			return 0;
//		}
//		
//	}	
//	
//	return _Mtemp;
//}
//

int main (void)
{
	Mindwave test;

	test.connect();

	int temp = 0; 
	while(temp < 1000)
	{
		//system("cls");
		// Get the most recent values of attention 
		test.updateAttention();
		temp = test.getAttention();
		printf("New ATT value: %d\n", temp);

		// Get the most recent values for meditation
		test.updateMeditation();
		temp = test.getMeditation();
		printf("New MED value: %d\n", temp);


		// Get the most recent values fore blink
		test.updateBlink();
		temp = test.getBlink();
		printf("New BLK value: %d\n", temp);

		//wait();

	}

	return 0; 
}
