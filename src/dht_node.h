#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using std::string_view;

class DHTNode
{
public:
	DHTNode(string_view hostname, uint16_t port, std::optional<size_t> node_id = {});

	void join_net(DHTNode const & bootstrap_node);

	void put(string_view key, string_view value);

	auto get(string_view key) const -> std::optional<string_view>;

	auto operator[](string_view key) const -> std::optional<string_view>
	{
		return this->get(key);
	}

	void add_neighbour(DHTNode const & new_node);

	void print() const;

private:
	struct NodeInfo
	{
		std::string host;
		size_t node_id;
		uint16_t port;

		auto operator==(NodeInfo const & b) const
		{
			return (this->port == b.port) && (this->node_id == b.node_id) &&
			       (this->host == b.host);
		};
	};
	NodeInfo m_info;

	std::map<std::string, std::string> m_data;

	std::vector<NodeInfo> m_neighbours;

	auto get_from_network_(string_view key) const -> std::optional<string_view>;

	void put_into_network_(string_view key, string_view value);
};
