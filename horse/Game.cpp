#include "game.h"

template<class S>
Game<S>::Game(const tcp::endpoint& ep,short threadnum)
	:base<S>(ep,threadnum)
{

}

template<class S>
void Game<S>::run()
{
	base::run();

	/// other logic 
}