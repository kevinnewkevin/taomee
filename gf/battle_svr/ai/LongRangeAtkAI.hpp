/**
 *============================================================
 *  @file      LongRangeAtkAI.hpp
 *  @brief    all states of a player are defined here
 * 
 *  compiler   gcc4.1.2
 *  platform   Linux
 *
 *  copyright:  TaoMee, Inc. ShangHai CN. All rights reserved.
 *
 *============================================================
 */

#ifndef KFBTL_LONGRANGEATK_AI_HPP_
#define KFBTL_LONGRANGEATK_AI_HPP_

extern "C" {
#include <libtaomee/log.h>
}

class MonsterAI;
class Player;

class LongRangeAtkAI : public MonsterAI {
public:
	~LongRangeAtkAI() {}
	
	void wait(Player* player, int millisec);
	void linger(Player* player, int millisec);
	void move(Player* player, int millisec);
	void evade(Player* player, int millisec);
	void attack(Player* player, int millisec);
	void stuck(Player* player, int millisec);
	void defense(Player* player, int millisec);
	
	//void UpdateAI(uint32 millisec );

	/**
	  * @brief this is a singleton
	  */
	static LongRangeAtkAI* instance();
	
private:
	LongRangeAtkAI(){ TRACE_LOG("LongRangeAtkAI create"); }
};

inline LongRangeAtkAI*
LongRangeAtkAI::instance()
{
	static LongRangeAtkAI instance;

	return &instance;
}

#endif // KFBTL_LONGRANGEATK_AI_HPP_