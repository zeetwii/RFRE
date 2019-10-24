/////////////////////////////////////////////////////////
//Code developed to generate ADS-B messages//////////////
/////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iomanip>

using namespace std;

long int completeMessageOdd[5];
long int completeMessageEven[5];
int Dlat0 = 6; //even messages
double Dlat1 = 6.101694915254237288135593220339; //odd messages
double pi = 3.14159265;
char buffer[30] = {0xA1, 0x40};

double Modulus(double val, double modval)
{
    if (val < 0) //Checking for negative angles
    {
        val = val + 360;
        int result = floor(static_cast<int>( val / modval));
        return val - static_cast<double>( result ) * modval;
    }
    else
    {
        int result = static_cast<int>( val / modval);
        return val - static_cast<double>( result ) * modval;
    }   
}

int GenerateNlatValue(double latitude)
{
    double Nlat;
    double sqrtHold;
    double buff[63];
    int j = 2;
    
    for (int i = 2; i <=59; i++)
    {
        Nlat = 0.0;
        sqrtHold = 0.0;
        Nlat = (180/pi)*(acos(sqrt((1-(cos(pi/30)))/(1-(cos((2*pi)/i))))));
        buff[j] = Nlat;
        j++;
    }

    for (int k = 59; k >=2; k--)
    {
        if(latitude > buff[k])
        {
            //do nothing
        }
        else
        {
            if (k < 2)
            {
                return 1;
            }
            
            return k;
        }
    }
}


int CalculateLatBitsOdd(double lat, double longitude)
{
    //Calculate YZ which will be what is put into our message
    double YZ;
    double modHold;
    int LatHex;

    modHold = Modulus(lat, Dlat1);
    modHold = (modHold/Dlat1);
    modHold = modHold * pow(2,17);
    modHold = modHold + .5;
    YZ = floor(modHold);

    //Calculate Rlatiude for airborne
    long double Rlat1;
    int floorHold;
    
    Rlat1 = (double)YZ / pow(2,17);
    floorHold = (lat/(double)Dlat1);
    floorHold = floor(floorHold);
    Rlat1 = Rlat1 + (double)floorHold;
    Rlat1 = Rlat1 * Dlat1;

    int NlLat = GenerateNlatValue(Rlat1);
    
    //Calcule DLongitude
    double Dlon1;

    if ((NlLat - 1) > 0)
    {
        Dlon1 = (360/((double)NlLat-1));
    }
    else
        Dlon1 = 360;

    //Calculate XZ the decimal representation of our longitude
    double XZ;

    modHold = 0;
    modHold = Modulus(longitude, Dlon1);

    modHold = (modHold/Dlon1);
    modHold = modHold * pow(2,17);
    modHold = modHold + .5;
    XZ = floor(modHold);

    //Ensure this fits into our 17 bit space
    long int YZ1 = Modulus(YZ,pow(2,17));
    long int XZ1 = Modulus(XZ,pow(2,17));
    
    long int LatLon;
    
    YZ1 = YZ1 << 17;
    LatLon = (YZ1 | XZ1);
    LatLon = (LatLon | 17179869184);
    completeMessageOdd[2] = LatLon;
    
    return 0;
}

int CalculateLatBitsEven()
{
    double lat;
    double longitude;
    
    cout << "Please enter your requested Latitude: \n";
    cin >> setbase(10) >> lat;
    cout << "Please enter your requested Longitude: \n";
    cin >> setbase(10) >> longitude;

    CalculateLatBitsOdd(lat, longitude);

    //Calculate YZ which will be what is put into our message
    double YZ;
    double modHold;
    int LatHex;
    
    modHold = Modulus(lat, Dlat0);
    modHold = (modHold/Dlat0);
    modHold = modHold * pow(2,17);
    modHold = modHold + .5;
    YZ = floor(modHold);

    //Calculate Rlatiude for airborne
    long double Rlat0;
    int floorHold;
    
    Rlat0 = (double)YZ / pow(2,17);
    floorHold = floor(lat/(double)Dlat0);
    Rlat0 = (double)Rlat0 + floorHold;
    Rlat0 = Rlat0 * (double)Dlat0;
    int NlLat = GenerateNlatValue(Rlat0);

    //Calcule DLongitude
    double Dlon0;

    if ((NlLat - 1) > 0)
    {
        Dlon0 = (360/((double)NlLat));
    }
    else
    {
        Dlon0 = 360;
    }

    //Calculate XZ the decimal representation
    //of our longitude
    double XZ;
    
    modHold = Modulus(longitude, Dlon0);
    modHold = (modHold/Dlon0);
    modHold = modHold * pow(2,17);
    modHold = modHold + .5;
    XZ = floor(modHold);

//ensure they will fit into a 17 bit message
long int YZ1 = Modulus(YZ,pow(2,17));
long int XZ1 = Modulus(XZ,pow(2,17));
long int LatLon;
YZ1 = YZ1 << 17;
LatLon = (YZ1 | XZ1);
completeMessageEven[2] = LatLon;
return 0;
}
long int CalculateAltitude()
{
long int altitude;
long int hold;
cout << "Please enter an altitude (0ft - 50,000ft): \n";
cin >> setbase(10) >> altitude;
altitude = (altitude + 1000)/25;
hold = (altitude & 0x00F);
altitude = (altitude & 0xFF0) << 1;
altitude = (altitude | hold); //concatenate to the entire message
altitude = (altitude | 0x010);
altitude = (altitude | 0x58000); //Takes on the TC (0x58) field
return altitude;
}
long int GenerateAircraftID()
{
long int address;
long int df17;
cout << "Enter an aircraft ID 3 Bytes (Example: BEEF11): \n";
cin >> setbase(16);
cout << setbase(16);
cin >> address;
df17 = 0x8D;
df17 = df17 << 24;
df17 = (df17 | address);
return df17;
}

long int CalculateCRC112BitsOdd()
{
long poly = 0xFFFA0480;
long int a = completeMessageOdd[0];
long int a2 = completeMessageOdd[1];
long int a3 = completeMessageOdd[2];
int j;
long int b = a2;
long int c = a3;
long int d;
long int hold;
b = b << 12;
d = (c & 0xFFF000000);
d = d >> 24;
a2 = (b | d);
hold = (a3 & 0x000FFFFFF);
hold = hold << 8;
a3 = hold;
for (j=1; j <= 88; j++)
{
if((a & 0x80000000) != 0)
{
a = a ^ poly;
}
a = a << 1;
 if((a2 & 0x80000000) != 0)
{
a = a|1;
}
a2 = a2 << 1;
 if((a3 & 0x80000000) != 0)
{
a2 = a2|1;
}
a3 = a3 << 1;
}
completeMessageOdd[3] = a;
return 0;
}
long int CalculateCRC112BitsEven()
{
long poly = 0xFFFA0480;
long int a = completeMessageEven[0];
long int a2 = completeMessageEven[1];
long int a3 = completeMessageEven[2];
int j;
long int b = a2;
long int c = a3;
long int d;
long int hold;

b = b << 12; //Bit manipulation to push the message together
d = (c & 0xFFF000000);
d = d >> 24;
a2 = (b | d);
hold = (a3 & 0x000FFFFFF);
hold = hold << 8;
a3 = hold;
for (j=1; j <= 88; j++)
{
if((a & 0x80000000) != 0)
{
a = a ^ poly;
}
a = a << 1;
 if((a2 & 0x80000000) != 0)
{
a = a|1;
}
a2 = a2 << 1;
 if((a3 & 0x80000000) != 0)
{
a2 = a2|1;
}
a3 = a3 << 1;
}
completeMessageEven[3] = a;
return 0;
}
long int ConvertForHexEditor(long int a, long int b, long int c, long
int d)
{
long int ACID = a;
int i = 1;
int m = 2;
while (i<=8)
{
ACID = (ACID & 0xF0000000);
if (ACID == 0x00000000)
buffer[m] = 0x55;
else if (ACID == 0x10000000)
buffer[m] = 0x56;
else if (ACID == 0x20000000)
buffer[m] = 0x59;
else if (ACID == 0x30000000)
buffer[m] = 0x5A;
else if (ACID == 0x40000000)
buffer[m] = 0x65;
else if (ACID == 0x50000000)
buffer[m] = 0x66;
else if (ACID == 0x60000000)
buffer[m] = 0x69;
else if (ACID == 0x70000000)

buffer[m] = 0x6A;
else if (ACID == 0x80000000)
buffer[m] = 0x95;
else if (ACID == 0x90000000)
buffer[m] = 0x96;
else if (ACID == 0xA0000000)
buffer[m] = 0x99;
else if (ACID == 0xB0000000)
buffer[m] = 0x9A;
else if (ACID == 0xC0000000)
buffer[m] = 0xA5;
else if (ACID == 0xD0000000)
buffer[m] = 0xA6;
else if (ACID == 0xE0000000)
buffer[m] = 0xA9;
else if (ACID == 0xF0000000)
buffer[m] = 0xAA;
else
ACID = a;
ACID = (ACID << (i*4));
i++;
m++;
}
cout << " ";
ACID = b;
ACID = (ACID << 12);
i = 4;
while (i<=8)
{
ACID = (ACID & 0xF0000000);
if (ACID == 0x00000000)
buffer[m] = 0x55;
else if (ACID == 0x10000000)
buffer[m] = 0x56;
else if (ACID == 0x20000000)
buffer[m] = 0x59;
else if (ACID == 0x30000000)
buffer[m] = 0x5A;
else if (ACID == 0x40000000)
buffer[m] = 0x65;
else if (ACID == 0x50000000)
buffer[m] = 0x66;
else if (ACID == 0x60000000)
buffer[m] = 0x69;
else if (ACID == 0x70000000)
buffer[m] = 0x6A;
else if (ACID == 0x80000000)
buffer[m] = 0x95;
else if (ACID == 0x90000000)
buffer[m] = 0x96;
else if (ACID == 0xA0000000)
buffer[m] = 0x99;
else if (ACID == 0xB0000000)

buffer[m] = 0x9A;
else if (ACID == 0xC0000000)
buffer[m] = 0xA5;
else if (ACID == 0xD0000000)
buffer[m] = 0xA6;
else if (ACID == 0xE0000000)
buffer[m] = 0xA9;
else if (ACID == 0xF0000000)
buffer[m] = 0xAA;
else
ACID = b;
ACID = (ACID << (i*4));
i++;
m++;
}
cout << " ";
ACID = c;
i = 1;
while (i<=9)
{
ACID = (ACID & 0xF00000000);
if (ACID == 0x000000000)
buffer[m] = 0x55;
else if (ACID == 0x100000000)
buffer[m] = 0x56;
else if (ACID == 0x200000000)
buffer[m] = 0x59;
else if (ACID == 0x300000000)
buffer[m] = 0x5A;
else if (ACID == 0x400000000)
buffer[m] = 0x65;
else if (ACID == 0x500000000)
buffer[m] = 0x66;
else if (ACID == 0x600000000)
buffer[m] = 0x69;
else if (ACID == 0x700000000)
buffer[m] = 0x6A;
else if (ACID == 0x800000000)
buffer[m] = 0x95;
else if (ACID == 0x900000000)
buffer[m] = 0x96;
else if (ACID == 0xA00000000)
buffer[m] = 0x99;
else if (ACID == 0xB00000000)
buffer[m] = 0x9A;
else if (ACID == 0xC00000000)
buffer[m] = 0xA5;
else if (ACID == 0xD00000000)
buffer[m] = 0xA6;
else if (ACID == 0xE00000000)
buffer[m] = 0xA9;
else if (ACID == 0xF00000000)
buffer[m] = 0xAA;

else
ACID = c;
ACID = (ACID << (i*4));
i++;
m++;
}
cout << " ";
ACID = d;
i = 1;
while (i<=6)
{
ACID = (ACID & 0xF0000000);
if (ACID == 0x00000000)
buffer[m] = 0x55;
else if (ACID == 0x10000000)
buffer[m] = 0x56;
else if (ACID == 0x20000000)
buffer[m] = 0x59;
else if (ACID == 0x30000000)
buffer[m] = 0x5A;
else if (ACID == 0x40000000)
buffer[m] = 0x65;
else if (ACID == 0x50000000)
buffer[m] = 0x66;
else if (ACID == 0x60000000)
buffer[m] = 0x69;
else if (ACID == 0x70000000)
buffer[m] = 0x6A;
else if (ACID == 0x80000000)
buffer[m] = 0x95;
else if (ACID == 0x90000000)
buffer[m] = 0x96;
else if (ACID == 0xA0000000)
buffer[m] = 0x99;
else if (ACID == 0xB0000000)
buffer[m] = 0x9A;
else if (ACID == 0xC0000000)
buffer[m] = 0xA5;
else if (ACID == 0xD0000000)
buffer[m] = 0xA6;
else if (ACID == 0xE0000000)
buffer[m] = 0xA9;
else if (ACID == 0xF0000000)
buffer[m] = 0xAA;
else
ACID = d;

ACID = (ACID << (i*4));
i++;
m++;
}
cout << "\n";
}
long int GenerateCompleteDF17Message()
{
long int address;
long int altitude;
long int position;
long int crcBits;
//Generate Aircraft ID and store in our buffer
address = GenerateAircraftID();
completeMessageOdd[0] = address;
completeMessageEven[0] = address;
//Generate Altitude and store in our buffer
altitude = CalculateAltitude();
completeMessageOdd[1] = altitude;
completeMessageEven[1] = altitude;
//Generate the Latitude and Longitude Bits, functions will store values in our buffer
CalculateLatBitsEven();
//Generate the CRC bits and store them in our buffer
CalculateCRC112BitsOdd();
CalculateCRC112BitsEven();
//Can be used for troubleshooting purposes if you need to see the odd message remove comments from code below.
//cout << setbase(16) << "Odd Message : " << completeMessageOdd[0] << " " << completeMessageOdd[1] << " " << completeMessageOdd[2] << " " << //completeMessageOdd[3] << "\n";
//Convert for easy entry into hex file
ConvertForHexEditor(completeMessageEven[0],completeMessageEven[1]
,completeMessageEven[2],completeMessageEven[3]);
ofstream myfile;
myfile.open ("FinalMessage.bin", ios::binary | ios::app);
 myfile.write(buffer, 30);
 myfile.close();
system("dd if=/dev/zero of=zeroes.bin bs=10 count=10");
system("cat FinalMessage.bin zeroes.bin > SecondaryMessage.bin");
//Can be used for troubelshotting purposes if you need to see the even message.
cout << setbase(16) << "ADS-B Message : " << completeMessageEven[0] << " " << completeMessageEven[1] << " " <<  completeMessageEven[2] << " " << completeMessageEven[3] << "\n";
ConvertForHexEditor(completeMessageOdd[0],completeMessageOdd[1],
completeMessageOdd[2],completeMessageOdd[3]);
myfile.open ("SecondaryMessage.bin", ios::binary | ios::app);
 myfile.write(buffer, 30);
 myfile.close();
system("cat SecondaryMessage.bin zeroes.bin > FinalMessage.bin");
//clean up
system("rm SecondaryMessage.bin");
system("rm zeroes.bin");
cout << "\n";
cout << "The ADS-B Message has been generated and is ready for transmission." << "\n";
return 0;
}
int main()
{
int choice;
int exit = 0;
system("clear");
cout << "This program will generate a Mode-S Downlink Format 17 Message, also known\nas an Automatic Dependenet Surveillance Broadcast (ADS-B) Message.\n";
while(exit ==0)
{
cout << "Press 1 <ENTER> to begin message generator.\n";
cin >> choice;
if (choice == 1)
{
printf("########################################################### \n", GenerateCompleteDF17Message());
}
else
{
printf("Exiting.............\n");
exit = 1;
}
}
return 0;
}