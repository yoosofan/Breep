
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 Lucas Lazare.                                                                  //
// This file is part of Breep project which is released under the                                //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <network.hpp>
#include <tcp/network_manager.hpp>
#include <iostream>
#include <vector>

#include <boost/uuid/uuid_io.hpp>

#define forever for(;;)

void message_received(breep::tcp::default_network&, const breep::tcp::default_peer& source, const std::deque<uint8_t>& data, bool) {
	std::cout << '\r' << boost::uuids::to_string(source.id()).substr(0,4) << ": ";
	for (uint8_t a : data) {
		std::cout << static_cast<char>(a);
	}
	std::cout << std::endl;
}

void connection(breep::tcp::default_network&, const breep::tcp::default_peer& new_peer, unsigned short) {
	std::cout << "SYSTEM: " << boost::uuids::to_string(new_peer.id()).substr(0,4) << " connected!" << std::endl;
}

void disconnection(breep::tcp::default_network&, const breep::tcp::default_peer& peer) {
	std::cout << "SYSTEM: " << boost::uuids::to_string(peer.id()).substr(0,4) << " disconnected" << std::endl;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Usage: chat.elf <hosting port> [target ip] [target port]" << std::endl;
		return 1;
	}

	breep::tcp::default_network network((unsigned short)atoi(argv[1]));

	std::cout << "SYSTEM: " << boost::uuids::to_string(network.self().id()) << " is your identity" << std::endl;


	network.add_data_listener(&message_received);
	network.add_connection_listener(&connection);
	network.add_disconnection_listener(&disconnection);

	if (argc == 2) {
		network.run();
	} else if (argc == 4){
		boost::asio::ip::address address = boost::asio::ip::address::from_string(argv[2]);
		network.connect(address, static_cast<unsigned short>(atoi(argv[3])));
	} else {
		std::cout << "Usage: chat.elf <hosting port> [target ip] [target port]" << std::endl;
		return 1;
	}

	std::string ans;
	forever {
		std::getline(std::cin, ans);
		network.send_to_all(ans);
	};
}