#ifndef RF_PROTOCOL_H_
#define RF_PROTOCOL_H_

#include "PacketDefinition_Common.h"

#ifdef RF_PROTOCOL_NORDEND
#include "PacketDefinition_Nordend.h"
#include "ParameterDefinition_Nordend.h"
#endif /* RF_PROTOCOL_NORDEND */

#ifdef RF_PROTOCOL_FIREHORN
#include "PacketDefinition_Firehorn2.h"
#include "ParameterDefinition_Firehorn2.h"
#include "DownlinkCompression_Firehorn2.h"
#endif /* RF_PROTOCOL_FIREHORN */

#ifdef RF_PROTOCOL_ICARUS
#include "PacketDefinition_Icarus.h"
#include "ParameterDefinition_Icarus.h"
#endif /* RF_PROTOCOL_ICARUS */

#endif /* RF_PROTOCOL_H_ */
