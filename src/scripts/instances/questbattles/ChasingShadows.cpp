#include <ScriptObject.h>
#include <Territory/QuestBattle.h>
#include <Actor/Player.h>

using namespace Sapphire;

class ChasingShadows : public Sapphire::ScriptAPI::QuestBattleScript
{
private:
  static constexpr auto INIT_POP_BOSS = 3802315;
  static constexpr auto INIT_P_POP_IDA = 3802271;
  static constexpr auto INIT_P_POP_PAPARIMO = 3802272;
  static constexpr auto INIT_POP_ENEMY_B_01 = 4094218;
  static constexpr auto INIT_POP_ENEMY_B_02 = 4094220;
  static constexpr auto INIT_POP_ENEMY_B_03 = 4094232;
  static constexpr auto INIT_POP_ENEMY_B_04 = 4094233;
  static constexpr auto INIT_POP_ENEMY_B_05 = 4094474;
  static constexpr auto INIT_POP_ENEMY_B_06 = 4094475;
  static constexpr auto INIT_POP_ENEMY_A_01 = 4094238;
  static constexpr auto INIT_POP_ENEMY_A_02 = 4094239;
  static constexpr auto INIT_POP_ENEMY_A_03 = 4094240;
  static constexpr auto INIT_POP_ENEMY_A_04 = 4094471;
  static constexpr auto INIT_POP_ENEMY_A_05 = 4094472;
  static constexpr auto INIT_POP_ENEMY_A_06 = 4094473;
  static constexpr auto CUT_SCENE_01 = 54;
  static constexpr auto HOW_TO_QIB = 79;

public:
  ChasingShadows() : Sapphire::ScriptAPI::QuestBattleScript( 11 )
  { }

  void onPlayerSetup( Sapphire::QuestBattle& instance, Entity::Player& player )
  {
    player.setRot( 3.f );
    player.setPos( { 323.f, -1.28f, -320.f } );
  }

  void onInit( QuestBattle& instance ) override
  {
    instance.registerEObj( "unknown_0", 2005192, 5760474, 4, { -51.493111f, 0.309087f, 71.436897f }, 1.000000f, -0.000006f ); 

  }

  void onUpdate( QuestBattle& instance, uint64_t tickCount ) override
  {

  }

  void onEnterTerritory( QuestBattle& instance, Entity::Player& player, uint32_t eventId, uint16_t param1,
                         uint16_t param2 ) override
  {
    player.playScene( instance.getDirectorId(), 1,
                      NO_DEFAULT_CAMERA | CONDITION_CUTSCENE | SILENT_ENTER_TERRI_ENV |
                      HIDE_HOTBAR | SILENT_ENTER_TERRI_BGM | SILENT_ENTER_TERRI_SE |
                      DISABLE_STEALTH | 0x00100000 | LOCK_HUD | LOCK_HOTBAR |
                      // todo: wtf is 0x00100000
                      DISABLE_CANCEL_EMOTE, 0 );
  }

  void onDutyComplete( QuestBattle& instance, Entity::Player& player ) override
  {
    player.updateQuest( instance.getQuestId(), 2 );
  }

};

EXPOSE_SCRIPT( ChasingShadows );