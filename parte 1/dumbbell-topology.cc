/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <cmath>
#include <iostream>
#include <sstream>

//ns3 includes
#include "ns3/log.h"
#include "ns3/point-to-point-dumbbell.h" /*modulo para conexion point to point con 
										topologia dumbbell*/
#include "ns3/constant-position-mobility-model.h"

#include "ns3/node-list.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/vector.h"

#include "ns3/netanim-module.h"
#include "ns3/onoff.aplication.h" /*se incluye la clase on/off aplication la cual 
									genera trafico a un unico destino segun un 
									patron OnOff*/


NS_LOG_COMPONENT_DEFINE ("dumbell topology parte 1");

namespace ns3{
PointToPointDumbbellHelper::PointToPointDumbbellHelper(uint32_t nLeftLeaf,
														PointToPointHelper leftHelper,
														uint32_t nRightLeaf,
														PointToPointHelper rightHelper
														PointToPointHelper bottleneckHelper)
{
	//Se crean los 2 nodos intermedios
	m_routers.Create(2);
	//Se crean los receptores de los extremos, derecha e izquierda
	m_leftLeaf.Create (nLeftLeaf); 
	m_rightLeaf.Create (nRightLeaf);
	
	//Se agrega la conexion entre los routers
	m_routerDevices = bottleneckHelper.Install (m_routers);
	//Se agrega las conexiones izquierdas
	for (uint32_t i=0; i<nLeftLeaf; i++)
		{
		NetDeviceContainer c = leftHelper.Install(m_routers.Get(0), m_leftLeaf(i));
		m_leftRouterDevices.Add(c.Get(0));
		m_leftLeafDevices.Add(c.Get(1));
	}
	//Se agrega las conexiones derechas
	for (uint32_t i=0; i<nRightLeaf; i++)
		{
		NetDeviceContainer c = RightHelper.Install(m_routers.Get(1), m_leftLeaf(i));
		m_RightRouterDevices.Add(c.Get(0));
		m_RightLeafDevices.Add(c.Get(1));
	}
}
	
