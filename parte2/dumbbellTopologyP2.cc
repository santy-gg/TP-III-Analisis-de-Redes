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
#include "ns3/string.h"
#include "ns3/core-module.h"
#include "ns3/point-to-point-dumbbell.h" /*modulo para conexion point to point con 
										topologia dumbbell*/
#include "ns3/point-to-point-helper.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/net-device-container.h" /*creará una instancia de un dispositivo de red, agregará una dirección MAC 
					y una cola al dispositivo y lo instalará en el nodo elegido*/
#include "ns3/node-list.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/vector.h"
#include "ns3/netanim-module.h"
#include "ns3/onoff-application.h" /*se incluye la clase on/off aplication la cual 
									genera trafico a un unico destino segun un 
									patron OnOff*/


NS_LOG_COMPONENT_DEFINE ("dumbell topology parte 1");

using namespace ns3;

int main(int argc, char *argv[])
{
     // Crear los nodos
    NodeContainer leftNodes, rightNodes, routers;
    leftNodes.Create(3); // 3 emisores
    rightNodes.Create(3); // 3 receptores
    routers.Create(2); // 2 nodos intermedios (routers)

    // Configurar el helper para PointToPoint
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    // Conectar los emisores al primer router
    NetDeviceContainer leftDevices, routerDevices1, routerDevices2, rightDevices;
    for (uint32_t i = 0; i < leftNodes.GetN(); ++i)
    {
        NetDeviceContainer link = pointToPoint.Install(leftNodes.Get(i), routers.Get(0));
        leftDevices.Add(link.Get(0));
        routerDevices1.Add(link.Get(1));
    }

    // Conectar los dos routers
    NetDeviceContainer routerLink = pointToPoint.Install(routers.Get(0), routers.Get(1));
    routerDevices1.Add(routerLink.Get(0));
    routerDevices2.Add(routerLink.Get(1));

    // Conectar el segundo router a los receptores
    for (uint32_t i = 0; i < rightNodes.GetN(); ++i)
    {
        NetDeviceContainer link = pointToPoint.Install(routers.Get(1), rightNodes.Get(i));
        routerDevices2.Add(link.Get(0));
        rightDevices.Add(link.Get(1));
    }

    // Instalar el stack de Internet en todos los nodos
    InternetStackHelper stack;
    stack.Install(leftNodes);
    stack.Install(routers);
    stack.Install(rightNodes);


    // Asignar direcciones IP
    Ipv4AddressHelper address;
    Ipv4InterfaceContainer leftInterfaces, routerInterfaces1, routerInterfaces2, rightInterfaces;

    address.SetBase("10.1.1.0", "255.255.255.0");
    leftInterfaces = address.Assign(leftDevices);
    routerInterfaces1 = address.Assign(routerDevices1);

    address.SetBase("10.1.2.0", "255.255.255.0");
    routerInterfaces2 = address.Assign(routerDevices2);

    address.SetBase("10.1.3.0", "255.255.255.0");
    rightInterfaces = address.Assign(rightDevices);

    // Configurar aplicaciones: un emisor UDP y dos TCP
    uint16_t port = 9;

    // Emisor UDP

    OnOffHelper udpClient("ns3::UdpSocketFactory", Address(InetSocketAddress(rightInterfaces.GetAddress(0), port)));
    udpClient.SetAttribute("DataRate", StringValue("5Mbps"));
    udpClient.SetAttribute("PacketSize", UintegerValue(1024));
    ApplicationContainer udpApps = udpClient.Install(leftNodes.Get(0));
    udpApps.Start(Seconds(1.0));
    udpApps.Stop(Seconds(10.0));

    // Receptor UDP

    PacketSinkHelper udpSink("ns3::UdpSocketFactory", Address(InetSocketAddress(Ipv4Address::GetAny(), port)));
    ApplicationContainer udpSinkApps = udpSink.Install(rightNodes.Get(0));
    udpSinkApps.Start(Seconds(0.0));
    udpSinkApps.Stop(Seconds(10.0));

    // Emisor TCP 1

    OnOffHelper tcpClient1("ns3::TcpSocketFactory", Address(InetSocketAddress(rightInterfaces.GetAddress(1), port)));
    tcpClient1.SetAttribute("DataRate", StringValue("5Mbps"));
    tcpClient1.SetAttribute("PacketSize", UintegerValue(1024));
    ApplicationContainer tcpApps1 = tcpClient1.Install(leftNodes.Get(1));
    tcpApps1.Start(Seconds(1.0));
    tcpApps1.Stop(Seconds(10.0));


    // Receptor TCP 1

    PacketSinkHelper tcpSink1("ns3::TcpSocketFactory", Address(InetSocketAddress(Ipv4Address::GetAny(), port)));
    ApplicationContainer tcpSinkApps1 = tcpSink1.Install(rightNodes.Get(1));
    tcpSinkApps1.Start(Seconds(0.0));
    tcpSinkApps1.Stop(Seconds(10.0));

    // Emisor TCP 2

    OnOffHelper tcpClient2("ns3::TcpSocketFactory", Address(InetSocketAddress(rightInterfaces.GetAddress(2), port)));
    tcpClient2.SetAttribute("DataRate", StringValue("5Mbps"));
    tcpClient2.SetAttribute("PacketSize", UintegerValue(1024));
    ApplicationContainer tcpApps2 = tcpClient2.Install(leftNodes.Get(2));
    tcpApps2.Start(Seconds(1.0));
    tcpApps2.Stop(Seconds(10.0));

    // Receptor TCP 2

    PacketSinkHelper tcpSink2("ns3::TcpSocketFactory", Address(InetSocketAddress(Ipv4Address::GetAny(), port)));
    ApplicationContainer tcpSinkApps2 = tcpSink2.Install(rightNodes.Get(2));
    tcpSinkApps2.Start(Seconds(0.0));
    tcpSinkApps2.Stop(Seconds(10.0));
    
    //inicio la aplicaion onOff con los nodos TCP
    tcpClient1.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]")); //tiempo de encendido TCP izquierdo
	tcpClient1.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=1]")); //tiempo de apagado TCP izquierdo
	tcpClient1.SetAttribute ("DataRate", DataRateValue (DataRate("500kbps")));//tasa de datos
	
	tcpClient2.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]")); //tiempo de encendido TCP derecho
	tcpClient2.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=1]")); //tiempo de apagado TCP derecho
	tcpClient2.SetAttribute ("DataRate", DataRateValue (DataRate("500kbps")));//tasa de datos
	
	ApplicationContainer apps1 = tcpClient1.Install(leftNodes.Get(0));
	apps1.Start(Seconds(1.0)); 
	apps1.Stop(Seconds(10.0));

	ApplicationContainer apps2 = tcpClient2.Install(rightNodes.Get(0));
	apps2.Start(Seconds(1.0)); 
	apps2.Stop(Seconds(10.0));

    // Habilitar la animación de la red (NetAnim)
    AnimationInterface anim("dumbbell_topology_parte2.xml");
	anim.SetConstantPosition (routers.Get(0), 30.5, 44.0);
	anim.SetConstantPosition (routers.Get(1), 60.5, 44.0);
	anim.SetConstantPosition (leftNodes.Get(0), 0.0, 0.0);
	anim.SetConstantPosition (leftNodes.Get(1), 0.0, 44.0);
	anim.SetConstantPosition (leftNodes.Get(2), 0.0, 88.0);
	anim.SetConstantPosition (rightNodes.Get(0), 96.0, 0.0);
	anim.SetConstantPosition (rightNodes.Get(1), 96.0, 44.0);
	anim.SetConstantPosition (rightNodes.Get(2), 96.0, 88.0);
    // Correr la simulación
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
