//All position values are * 10 to get better acuraccy

//Position of any object [ID,X,Y,Z,RX,RY,RZ,animation,mesh]
const int Sword_Object = 0;
//An effect [X,Y,Z,Type]
const int Sword_Animation = 1;
//A sound to be played [X,Y,Z,Type,volume]
const int Sword_Sound = 2;
//A player movement ie walk [dX,dY,dZ,dRX,dRY,dRZ]
const int Sword_MovePlayer = 3;
//An object movement ie Sword swing [dX,dY,dZ,dRX,dRY,dRZ]
const int Sword_MoveObject = 4;
//Id of the player Objects [TypeId,Id]
const int Sword_PlayerIds = 5;
//Camera is bound
const int Sword_PlayerId_Head = 0;
const int Sword_PlayerId_Body = 1;
const int Sword_PlayerId_ArmUpperR = 2;
const int Sword_PlayerId_ArmUpperL = 3;
const int Sword_PlayerId_ArmLowerR = 4;
const int Sword_PlayerId_ArmLowerL = 5;
const int Sword_PlayerId_LegUpperR = 6;
const int Sword_PlayerId_LegUpperL = 7;
const int Sword_PlayerId_LegLowerR = 8;
const int Sword_PlayerId_LegLowerL = 9;