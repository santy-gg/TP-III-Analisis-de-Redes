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
#include "ns3/net-device-container.h" /*creará una instancia de un dispositivo de red, agregará una dirección MAC 
					y una cola al dispositivo y lo instalará en el nodo elegido*/

#include "ns3/node-list.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/vector.h"

#include "ns3/netanim-module.h"
#include "ns3/onoff.aplication.h" /*se incluye la clase on/off aplication la cual 
									genera trafico a un unico destino segun un 
									patron OnOff*/


NS_LOG_COMPONENT_DEFINE ("dumbell topology parte 1");

namespace ns3;

int main(in argc, char *argv[])
{
	//crear los nodos
	NodeContainer leftNodes, rightNodes, routers; /*se instancian los nodos izquierods, derechos y routers*/
	leftNodes.Create(3); //los tres nodos izquierdos
	rightNodes.Create(3); //los tres nodos derechos
	routers.Create(2); //nodos intermedios (routers)

	//se agrega la conexion entre los routers
	m_routerDevices = bottleneckHelper.Install(routers);

	//configurar el helper para PointToPointDumbbell
	PointToPointDumbbellHelper pointToPointDumbbell;

	//conectar los nodos izquierdos
	for(uint32_t i = 0; i<leftNodes.GetLeft(); ++i)
		{
			NetContainer link = pointToPointDumbbell.leftHelper.Install(routers(0), leftNodes(i));
			m_leftRouterDevices.Add(link.Get(0));
			m_leftLeafDevices.Add(link.Get(1));
		}
	//conectar los nodos derechos
	for(uint32_t i = 0; i<rightNodes.GetRight(); ++i)
		{
			NetContainer link = pointToPointDumbbell.rightHelper.Install(routers(1), leftNodes(i));
			m_rightRouterDevices.Add(link.Get(0));
			m_rightLeafDevices.Add(link.Get(1));
		}
