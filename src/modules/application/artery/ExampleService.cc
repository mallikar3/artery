//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "ExampleService.h"
#include <vanetza/btp/data_request.hpp>
#include <vanetza/dcc/profile.hpp>
#include <vanetza/geonet/interface.hpp>

Define_Module(ExampleService);

using namespace vanetza;

ExampleService::ExampleService()
{
}

ExampleService::~ExampleService()
{
}

void ExampleService::indicate(const btp::DataIndication& ind, cPacket* packet)
{
	if (packet->getByteLength() == 42) {
		findHost()->bubble("packet indication");
	}
}

void ExampleService::initialize()
{
	ItsG5BaseService::initialize();
	m_self_msg = new cMessage("Example Service");

	scheduleAt(simTime() + 3.0, m_self_msg);
}

void ExampleService::finish()
{
	cancelAndDelete(m_self_msg);
}

void ExampleService::handleMessage(cMessage* msg)
{
	Enter_Method("handleMessage");
	if (msg == m_self_msg) {
		findHost()->bubble("self message");
	}
}

void ExampleService::trigger()
{
	Enter_Method("trigger");
	btp::DataRequestB req;
	req.destination_port = host_cast<ExampleService::port_type>(getPortNumber());
	req.gn.transport_type = geonet::TransportType::SHB;
	req.gn.traffic_class.tc_id(static_cast<unsigned>(dcc::Profile::DP3));
	req.gn.communication_profile = geonet::CommunicationProfile::ITS_G5;

	cPacket* packet = new cPacket();
	packet->setByteLength(42);
	request(req, packet);
}