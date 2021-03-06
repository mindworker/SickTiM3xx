/*
 * CAN_EDS2SDO_Factory.h
 *
 *  Created on: 01.04.2010
 *      Author: locher
 */

#ifndef CAN_EDS2SDO_FACTORY_H_
#define CAN_EDS2SDO_FACTORY_H_

#include <auto_ptr.h>


#include "CAN_EDS_SDO.h"

namespace sfzcan {






	/**
	 * \brief The CAN_Config_EDS Class is used to config one of the
	 * Makro generated general EDS_SDO Objekts by calling the static Method get_EDS
	 *
	 */

	class CAN_Config_EDS {
	public:
		CAN_Config_EDS();
		virtual ~CAN_Config_EDS();

		/**
		 *
		 * Calling get_EDS
		 *
		 * @param[in] CAN_EDS_SDO An EDS_SDO Object. Use one of the Autogenerated makros to generate the Object
		 * @param[in] canid_t the can-node-id where the EDS-SDO goes
		 * @param[in] CAN_EDS_SDO::READ_WRITE_STATE send or receive the Data
		 * @param[in] msgdata_t optional value containing the data to send
		 */

	static std::auto_ptr<CAN_EDS_SDO> get_EDS(
			CAN_EDS_SDO eds,
			const canid_t nodeID,
			const CAN_EDS_SDO::READ_WRITE_STATE=CAN_EDS_SDO::READ_WRITE_STATE_READ,
			const msgdata_t msgdata=0)throw(E_SDO_Exception) ;
	};


}  // namespace sfzcan


#endif /* CAN_EDS2SDO_FACTORY_H_ */
