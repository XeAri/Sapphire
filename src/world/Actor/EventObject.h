#ifndef SAPPHIRE_INSTANCEOBJECT_H
#define SAPPHIRE_INSTANCEOBJECT_H

#include "Actor.h"

namespace Sapphire::Entity
{

  class EventObject : public Actor
  {
  public:
    EventObject( uint32_t actorId, uint32_t objectId, uint32_t gimmickId, uint8_t initialState,
                 Common::FFXIVARR_POSITION3 pos, float rotation, const std::string& givenName = "none" );

    using OnTalkEventHandler = std::function< void( Entity::Player&, Entity::EventObjectPtr,
                                                    ZonePtr, uint64_t ) >;

    uint32_t getGimmickId() const;

    void setGimmickId( uint32_t gimmickId );

    uint8_t getState() const;

    void setState( uint8_t state );

    float getScale() const;

    void setScale( float scale );

    void setOnTalkHandler( OnTalkEventHandler handler );

    OnTalkEventHandler getOnTalkHandler() const;

    uint32_t getObjectId() const;

    const std::string& getName() const;

    ZonePtr getParentInstance() const;

    void setParentInstance( ZonePtr instance );

    void spawn( PlayerPtr pTarget ) override;

    void despawn( PlayerPtr pTarget ) override;

    void setAnimationFlag( uint32_t flag, uint32_t animationFlag );

    void setHousingLink( uint32_t housingLink );

    uint32_t getHousingLink() const;

  protected:
    uint32_t m_housingLink;
    uint32_t m_gimmickId;
    uint32_t m_objectId;
    uint8_t m_state;
    float m_scale;
    std::string m_name;
    ZonePtr m_parentInstance;
    OnTalkEventHandler m_onTalkEventHandler;


  };
}

#endif //SAPPHIRE_INSTANCEOBJECT_H
