#ifndef ENUMS_HPP
#define ENUMS_HPP

#pragma once

enum SendPropType { // already had it
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
	DPT_String,
	DPT_Array,	// An array of the base types (can't be of datatables).
	DPT_DataTable,
	DPT_Quaternion,
	DPT_Int64,
	DPT_NUMSendPropTypes
};

enum LifeState_t {
	LIFE_ALIVE = 0,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY
};

enum EventDebugID {
	EVENT_SHUTDOWN = 13,
	EVENT_INIT = 42
};

#endif // !ENUMS_HPP