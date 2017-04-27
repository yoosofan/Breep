#ifndef BREEP_IO_MANAGER_BASE_HPP
#define BREEP_IO_MANAGER_BASE_HPP


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 Lucas Lazare.                                                                  //
// This file is part of Breep project which is released under the                                //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @file invalid_state.hpp
 * @author Lucas Lazare
 *
 * @since 0.1.0
 */

#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash.hpp>

#include "commands.hpp"
#include "detail/utils.hpp"

namespace boost::asio::ip {
	class address;
}

namespace breep {

	template <typename T>
	class network_manager;

	template <typename T>
	class peer;

	/**
	 * @class network_manager_base network_manager_base.hpp
	 * @brief base class for network managers, used by \em network<typename network_manager>.
	 * @details Classes inheriting from network_manager_base should specify a socket_type type and a buffer_length size.
	 * The object should open ports and start listening for incoming connections as soon as they get an owner.
	 * The constructor should take an unsigned short [port] as parameter if instantiated from the class breep::network.
	 *
	 * @sa breep::network
	 *
	 * @since 0.1.0
	 */
	template <typename io_manager>
	class io_manager_base {
	public:

		virtual ~io_manager_base() {

		}

		/**
		 * @brief Sends data to a peer
		 *
		 * @tparam data_container Any data container type you want to support. In the case of \em tcp_nmanager
		 *                        and \em udp_nmanager, the data_container type should respect the \em Container concept
		 *                        (for \em uint8_t).
		 *
		 * @param command command of the packet (considered as data)
		 * @param data data to be sent. It is uppon your responsability to check for endianness.
		 * @param peer the peer to whom to send the data.
		 */
		template <typename data_container>
		void send(commands command, const data_container& data, const peer<io_manager>& peer) const {
			static_assert(detail::dependent_false<io_manager_base<io_manager>, data_container>::value, "Send called without specialisation.");
		}

		/**
		 * @brief Sends data to a peer
		 *
		 * @tparam data_iterator Any data iterator type you want to support. In the case of \em tcp_nmanager
		 *                        and \em udp_nmanager, the data_iterator type should respect the \em InputIterator concept.
		 *                        (for \em uint8_t).
		 * @tparam size_type
		 *
		 * @param command command of the packet (considered as data)
		 * @param begin iterator to the data to be sent. It is uppon your responsability to check for endianness.
		 * @param size quantity of data to be sent
		 * @param peer of the peer to whom to send the data.
		 */
		template <typename data_iterator, typename  size_type>
		void send(commands command, data_iterator begin, size_type size, const peer<io_manager>& peer) const {
			static_assert(detail::dependent_false<io_manager_base<io_manager>, data_iterator>::value, "Send called without specialisation.");
		}

		/**
		 * @brief connects to a peer
		 *
		 * @return the newly connected peer or peer::bad_peer if the connection wasn't successful.
		 */
		virtual peer<io_manager> connect(const boost::asio::ip::address&, unsigned short port) = 0;

		/**
		 * @brief performs any required action after a peer connection.
		 */
		virtual void process_connected_peer(peer<io_manager>& peer) = 0;

		/**
		 * @brief disconnects from the network
		 */
		virtual void disconnect() = 0;

		/**
		 * @brief Network's main thread entry point
		 */
		virtual void run() = 0;

	protected:

		/**
		 * @brief sets the port
		 * 			Will never be called from the owner if the network is active.
		 * @param port the new port.
		 */
		virtual void port(unsigned short port) = 0;

		/**
		 * @brief sets the owner of the network_manager, ie the object to whom received datas should be redirected.
		 */
		virtual void owner(network_manager<io_manager>* owner) = 0;

		friend class network_manager<io_manager>;
	};
}

#endif //BREEP_IO_MANAGER_BASE_HPP