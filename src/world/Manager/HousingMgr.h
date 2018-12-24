#ifndef SAPPHIRE_HOUSINGMGR_H
#define SAPPHIRE_HOUSINGMGR_H

#include "Forwards.h"
#include "Territory/HousingZone.h"
#include <set>
#include <unordered_map>

namespace Sapphire::Data
{
  using HousingZonePtr = std::shared_ptr< HousingZone >;
}

namespace Sapphire::World::Manager
{
  class HousingMgr
  {

  public:

    /*!
     * @brief Structure that is generated and filled on world launch. Used to store housing data during init
     * so we don't need to query them individually.
     */
    struct LandCacheEntry
    {
      // land table
      uint64_t m_landSetId;
      uint64_t m_landId;

      Common::LandType m_type;
      uint8_t m_size;
      uint8_t m_status;

      uint64_t m_currentPrice;

      uint64_t m_updateTime;
      uint64_t m_ownerId;
      uint64_t m_houseId;

      // house table

      std::string m_estateWelcome;
      std::string m_estateComment;
      std::string m_estateName;

      uint64_t m_buildTime;
      uint64_t m_endorsements;
    };

    /*!
     * @brief Maps land id to a list of cached entries
     */
    using LandSetLandCacheMap = std::unordered_map< uint64_t, std::vector< LandCacheEntry > >;

    /*!
     * @brief Maps container IDs to their relevant ItemContainerPtr
     */
    using ContainerIdToContainerMap = std::unordered_map< uint16_t, ItemContainerPtr >;
    /*!
     * @brief Maps land idents to a container containing ItemContainerPtrs
     */
    using LandIdentToInventoryContainerMap = std::unordered_map< uint64_t, ContainerIdToContainerMap >;

    HousingMgr();
    virtual ~HousingMgr();

    bool init();

    uint32_t toLandSetId( uint16_t territoryTypeId, uint8_t wardId ) const;
    Sapphire::Data::HousingZonePtr getHousingZoneByLandSetId( uint32_t id );
    Sapphire::LandPtr getLandByOwnerId( uint32_t id );

    void sendLandSignOwned( Entity::Player& player, const Common::LandIdent ident );
    void sendLandSignFree( Entity::Player& player, const Common::LandIdent ident );
    LandPurchaseResult purchaseLand( Entity::Player& player, uint8_t plot, uint8_t state );

    /*!
     * @brief Converts param1 of a client trigger into a Common::LandIndent
     */
    Common::LandIdent clientTriggerParamsToLandIdent( uint32_t param11, uint32_t param12, bool use16bits = true ) const;

    void sendWardLandInfo( Entity::Player& player, uint8_t wardId, uint16_t territoryTypeId );

    bool relinquishLand( Entity::Player& player, uint8_t plot );

    void buildPresetEstate( Entity::Player& player, uint8_t plotNum, uint32_t presetItem );

    void requestEstateRename( Entity::Player& player, const Common::LandIdent ident );

    void requestEstateEditGreeting( Entity::Player& player, const Common::LandIdent ident );
    void updateEstateGreeting( Entity::Player& player, const Common::LandIdent ident, const std::string& greeting );

    void requestEstateEditGuestAccess( Entity::Player& player, const Common::LandIdent ident );

    void sendEstateGreeting( Entity::Player& player, const Common::LandIdent ident );

    /*!
     * @brief Updates the cached models on a house from the relevant apperance inventories.
     * Does not send the subsequent update to clients.
     *
     * @param house The house to update the models for
     */
    void updateHouseModels( HousePtr house );

    /*!
     * @brief Sends the house inventory for the specified type to a player.
     *
     * This enforces permissions on the inventory too so random players can't request an estates items
     */
    void sendEstateInventory( Entity::Player& player, uint16_t inventoryType, uint8_t plotNum );

    /*!
     * @brief Get the land & house data that was cached on world startup.
     * @return
     */
    const LandSetLandCacheMap& getLandCacheMap();

    /*!
     * @brief Get all loaded inventories for housing estates
     * @return
     */
    LandIdentToInventoryContainerMap& getEstateInventories();

    /*!
     * @brief Get an estate inventory for a specific estate
     * @param ident LandIdent for the specified estate
     * @return A map containing container ids to ItemContainerPtr
     */
    ContainerIdToContainerMap& getEstateInventory( uint64_t ident );

    /*!
     * @brief Get an estate inventory for a specific estate
     * @param ident LandIdent for the specified estate
     * @return A map containing container ids to ItemContainerPtr
     */
    ContainerIdToContainerMap& getEstateInventory( Common::LandIdent ident );

    /**
     * @brief Sets up inventories and spawns the base items for the house appearance
     * @param land The house to update
     */
    bool initHouseModels( Entity::Player& player, LandPtr land, uint32_t presetCatalogId );

  private:

    /*!
     * @brief Creates a house and saves the minimum amount required to persist a house through restarts.
     *
     * Any other changes will be covered by the usual saving logic and can be safely ignored here.
     *
     * @param house The house to create in the house table
     */
    void createHouse( HousePtr house ) const;

    /*!
     * @brief Gets the next available house id
     * @return The next available house id
     */
    uint64_t getNextHouseId();

    /*!
     * @brief Loads all the land entries from the database and caches them to speed up housing territory init
     */
    void loadLandCache();

    /*!
     * @brief Loads all the inventories for every estate on the world and sets up their containers
     * @return True if it was successful
     */
    bool loadEstateInventories();

    /*!
     * @brief Gets the additionalData field from item.exd for an item
     * @param catalogId The item id to lookup in item.exd
     * @return The additionalData field from item.exd
     */
    uint32_t getItemAdditionalData( uint32_t catalogId );

    LandSetLandCacheMap m_landCache;
    LandIdentToInventoryContainerMap m_estateInventories;

  };

}

#endif // SAPPHIRE_HOUSINGMGR_H
