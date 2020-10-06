#include <stdio.h>
#include <cerrno>
#include <assert.h>
#include <SFML/Graphics.hpp>

#include <vector>

#include "myssh.h"


#ifndef __FILENAME__
#define __FILENAME__ strchr(__FILE__, '/') ? strchr(__FILE__, '/')  + 1 : __FILE__ 
#endif
#define ERRPRINTF(format, ...)	fprintf(stderr, "%d::%s::%s__::__ " format "\n", __LINE__, __FILENAME__, __PRETTY_FUNCTION__, ## __VA_ARGS__)


typedef int errno_t;

namespace AS_SFML_SETTINGS
{
	
}

enum AS_CLASS_ELEM
{
	AS_TYPE_SORTS,
	AS_TYPE_FILLS,
	AS_TYPE_BRIEF,
	AS_TYPE_SORTS_DEF,
	AS_TYPE_FILLS_DEF
};

enum AS_ID_ELEM
{
	AS_ID_SEND
};

void (*func_btns())[] = {
	
};

class Button
{
	
};

class Text
{
	
};

class Graph
{
	
};


typedef struct AS_elemets_container
{
	std::vector<Button>	btns;
	std::vector<Text>	texts;
	std::vector<Graph>	graphs;
} AS_elems_t;

typedef struct AS_data_to_send
{
	bool sortings;
	bool fills;
	size_t num;
} AS_data_t;

typedef struct AS_bool_send_data
{
	bool isToSend;
} AS_bool_send_t;

errno_t build_app(LIBSSH2_SESSION* session,
					sf::RenderWindow* window, sf::RenderWindow *warning_window,
					AS_elems_t* elements, AppContext* appctx)
{
	assert(window != nullptr);
	assert(warning_window != nullptr);
	
	assert(elements != nullptr);
	
	assert(appctx != nullptr);
	
	ssize_t ret = 0;
	
	AS_data_t data = {};
	Button *send_btn = (Button*)get_elem_by_id(AS_ID_SEND);
	
	while (window->isOpen())
	{
		sf::Event event{};
		while (window->pollEvent(event))
		{
			if  (event.type == sf::Event::Closed ||
				(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
			{
				window->close();
			}
		}
		
		run_clcked_buttons(appctx->btns);
		
		if (send_btn->priv_data && (AS_bool_send_t*)(send_btn->priv_data)->isToSend) // Додумать обращение
		{
			(AS_bool_send_t*)(send_btn->priv_data)->isToSend = false;
			ret = send(socket_data.get_sock_fd(), (const void*)(&data), sizeof(data), 0);
			if (ret == -1)
			{
				return errno;
			}
		}
	}
}


int main(int argc, char **argv)
{
	int ret = 0;
	
	
	
	LibSSH2& instance = LibSSH2::Instance();
	
	const char *hostname = "192.168.43.165";
	const char *username = "testssh";
	const char *password = "123";
	
	Socket sckt();
	
	struct sockaddr_in sin;
	
	Session session();
	SimpleChannel channel(sckt.get_sock_fd(), session.m_session); // get_m_session
	
	unsigned long hostaddr = inet_addr(hostname);
	sin.sin_port = htons(22);
	sin.sin_addr.s_addr = hostaddr;
	if (connect(sckt.get_sock_fd(), (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0)
	{
		perror("Bad connect: ");
		return -1;
	}
	
	while ((ret = libssh2_session_handshake(session.m_session, sckt.get_sock_fd())) == LIBSSH2_ERROR_EAGAIN)
	{
		if (ret)
		{
			ERRPRINTF("Bad session handshake");
			return -1;
		}
	}
	
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Analysis sortings");
	sf::RenderWindow warning_window(AS_SFML_SETTINGS::WarningWindowSize, "Analysis sortings");
	
	sf::Font font;
	if (font.loadFromFile(AS_SFML_SETTINGS::FontFile) == false)
	{
		ERRPRINTF("Bad loading fonts: %s", AS_SFML_SETTINGS::FontFile);
		return -1;
	}
	
	AS_elems_t* elements = { 0 };
	
	elements.texts.push_back(Text()); // brief_text
	
	elements.texts.push_back(Text()); // choose_sort_text
	
	sf::Vector2f pos_btns(0, 0);
	sf::Vector2f step_btns(0, 0);
	
	set_buttons(elements.btns, AS_SFML_SETTINGS::NumTypingSortings, // sortings_types_buttons
				AS_TYPE_SORTS,
				pos_btns, step_btns);
	
	set_buttons(elements.btns, AS_SFML_SETTINGS::NumTypingFillings, // fillings_types_buttons
				AS_TYPE_FILLS,
				pos_btns, step_btns);
	
	elements.btns.push_back(Button()); // Send
	
#if 0
	elements.graphs.push_back(Graph());
	elements.graphs.push_back(Graph());
#endif
	
	sf::Vector2f pos_graphs(0, 0);
	sf::Vector2f step_graphs(0, 0);
	
	set_graphs(elements.graphs, AS_SFML_SETTINGS::NumGraphs,
				AS_TYPE_GRAPHS,
				pos_graphs, step_graphs);
	
	ret = build_app(window, warning_window, elements, appctx);
	if (ret != 0)
	{
		perror("Bad build_app: ");
		return errno;
	}
	
	return 0;
}
