#include "dht_node.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <format>
#include <functional>
#include <string>

#include <print>

DHTNode::DHTNode(string_view hostname, uint16_t port, std::optional<size_t> node_id)
	: m_info{.host{hostname},
		 .node_id = node_id.value_or(
			 std::hash<std::string>{}(std::format("{}:{}", hostname, port))),
		 .port = port}
{
}

void DHTNode::add_neighbour(DHTNode const & new_node)
{
	auto const & info = new_node.m_info;
	if (std::ranges::any_of(this->m_neighbours, [&info](auto const & a) { return a == info; }))
		return;

	this->m_neighbours.push_back(info);
}

auto DHTNode::get(string_view key) const -> std::optional<std::string_view>
{
	try {
		return this->m_data.at(std::string{key});
	} catch (std::out_of_range &) {
		return this->get_from_network_(key);
	}
}

auto DHTNode::get_from_network_(string_view key) const -> std::optional<string_view>
{
	// todo: поиск в других нодах
	std::println(stderr, "Поиск в сети не реализован!");
	return {};
	(void)key;
}

static bool dont_store_in_this_node(size_t hash)
{
	// todo: нормальное правило распределения
	return hash % 2;
}

void DHTNode::put_into_network_(string_view key, string_view value)
{
	std::println(stderr, "Размещение элемента в сети не реализовано");
	(void)key;
	(void)value;
}

void DHTNode::put(string_view key, string_view value)
{
	auto hash = std::hash<std::string_view>{}(key);
	std::println("Добавление элемента ({}:{}), его хэш:{:x}", key, value, hash);
	if (dont_store_in_this_node(hash)) {
		this->put_into_network_(key, value);
		return;
	}

	this->m_data[std::string{key}] = value;
	std::println("Добавлен в ноду {:x}", this->m_info.node_id);
}

void DHTNode::print() const
{
	std::println("Адрес: {}:{}", this->m_info.host, this->m_info.port);
	std::println("ID ноды: {:x}", this->m_info.node_id);

	std::println("Данные в ноде:");
	for (auto const & kv : this->m_data) {
		std::println("\t{}: {}", kv.first, kv.second);
	}
	std::println();

	std::println("Соседние ноды:");
	for (auto const & n : this->m_neighbours) {
		std::println("\t{:x}", n.node_id);
	}
	std::println();
}
