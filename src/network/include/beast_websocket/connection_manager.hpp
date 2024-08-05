#pragma once

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/uuid/uuid_hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>


namespace beast_websocket
{
struct connection;

struct connection_manager
{
	connection_manager(connection_manager&&) = delete;
	static connection_manager& instance() noexcept;
	void add_conncetion(std::shared_ptr<connection> connection_ptr);
	void remove_conncetion(boost::uuids::uuid uuid);

private:
	connection_manager() noexcept = default;

	boost::unordered_flat_map<boost::uuids::uuid, std::shared_ptr<connection>> m_conncetions;
};

} // namespace beast_websocket