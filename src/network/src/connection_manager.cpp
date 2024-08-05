#include <beast_websocket/connection_manager.hpp>
#include <beast_websocket/connection.hpp>

namespace beast_websocket
{
connection_manager& connection_manager::instance() noexcept
{
	static connection_manager instance;
	return instance;
}

void connection_manager::add_conncetion(std::shared_ptr<connection> connection_ptr)
{
	m_conncetions.try_emplace(connection_ptr->get_uuid(), connection_ptr);
}

void connection_manager::remove_conncetion(boost::uuids::uuid uuid)
{
	m_conncetions.erase(uuid);
}

} // namespace beast_websocket