#include "dht_node.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <flat_map>
#include <print>
#include <string>
#include <string_view>
#include <vector>

class DHTDemo
{
private:
	std::vector<DHTNode> m_nodes;

	auto create_node_(std::string_view host, uint16_t port) -> DHTNode &
	{
		auto new_node = DHTNode(host, port);
		if (m_nodes.size()) {
			auto & bootstrap_node = *m_nodes.begin();

			// todo. Эта нода будет добавлятся дважды, т.к. в оригинале здесь метод
			// join_network
			new_node.add_neighbour(bootstrap_node);

			for (auto & old_node : this->m_nodes) {
				new_node.add_neighbour(old_node);
				old_node.add_neighbour(new_node);
			}
		}
		this->m_nodes.push_back(new_node);
		return *this->m_nodes.rbegin();
	}

	auto get_rand_node() -> DHTNode &
	{
		return this->m_nodes[(size_t)rand() % this->m_nodes.size()];
	}

public:
	DHTDemo() { srand((unsigned int)time(NULL)); }

	auto basic_operations()
	{
		auto n1 = this->create_node_("localhost", 8801);
		auto n2 = this->create_node_("localhost", 8802);
		auto n3 = this->create_node_("localhost", 8803);

		std::println("Созданы нод:");
		for (auto const & n : this->m_nodes) {
			n.print();
		}

		std::getchar();

		std::println("Добавление данных:");
		std::flat_map<std::string, std::string> test_data;
		test_data["user:1"] = "Алиса", test_data["user:2"] = "Боб",
		test_data["user:3"] = "Чарли", test_data["config:timeout"] = "30",
		test_data["cache:item1"] = "значение кеша";

		for (auto const & [k, v] : test_data) {
			auto & r_node = get_rand_node();
			r_node.put(k, v);
		}

		std::getchar();

		std::println("Поиск данных");
		std::array<std::string, 3> search_samples{"user:1", "user:3", "nonexistant"};
		for (auto & str : search_samples) {
			auto & r_node = get_rand_node();
			auto ret = r_node.get(str);
			if (ret) {
				std::println("По ключу {} найдено {}", str, *ret);
			} else {
				std::println("По ключу {} данных не найдено!", str);
			}
		}
		std::getchar();

		std::println("После манипуляций:");
		for (auto const & n : this->m_nodes) {
			n.print();
		}

		return *this;
	}

};

int main()
{
	DHTDemo().basic_operations();
}
