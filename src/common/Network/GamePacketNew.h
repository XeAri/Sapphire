#ifndef _GAMEPACKETNEW_H
#define _GAMEPACKETNEW_H

#include <stdint.h>
#include <iostream>

#include "GamePacket.h"

#include <sstream>
#include <time.h>

namespace Core {
namespace Network {
namespace Packets {

// Must forward define these in order to enable the compiler to produce the
// correct template functions.

template < typename T, typename T1 >
class GamePacketNew;

template < typename T, typename T1 >
class FFXIVIpcPacket;

template < typename T, typename T1 >
std::ostream& operator << ( std::ostream& os, const GamePacketNew< T, T1 >& packet );

template< class T >
using ZoneChannelPacket = FFXIVIpcPacket< T, ServerZoneIpcType >;

template< class T >
using ChatChannelPacket = FFXIVIpcPacket< T, ServerChatIpcType >;

template< class T >
using ZoneChannelPacketNew = FFXIVIpcPacket< T, ServerZoneIpcType >;

template< class T >
using ChatChannelPacketNew = FFXIVIpcPacket< T, ServerChatIpcType >;

/**
* The base implementation of a game packet. Needed for parsing packets.
*/
template < typename T1 >
class FFXIVIpcPacketBase
{
public:
   virtual ~FFXIVIpcPacketBase() = default;
   /**
   * @brief Gets the IPC type of this packet. (Useful for determining the
   * type of a parsed packet.)
   */
   virtual T1 ipcType() = 0;
};

/**
* A game packet, or IPC packet, object is a template class for constructing
* the data to be sent or parsed. The template works by accepting a structure
* type that represents just the IPC data portion (the bytes after the initial
* 32 byte header information.)
*/
template < typename T, typename T1 >
class GamePacketNew : public FFXIVIpcPacketBase< T1 >
{
public:
   /**
   * @brief Constructs a new game packet with the specified actors.
   * @param sourceActorId The source actor id.
   * @param targetActorId The target actor id.
   */
   GamePacketNew< T, T1 >( uint32_t sourceActorId, uint32_t targetActorId )
   {
      initialize();
      m_segHdr.source_actor = sourceActorId;
      m_segHdr.target_actor = targetActorId;
   };

   /**
   * @brief Constructs a new game packet with the specified actors.
   * @param sourceActorId The source and target actor id.
   */
   GamePacketNew< T, T1 >( uint32_t bothActorId )
   {
      initialize();
      m_segHdr.source_actor = bothActorId;
      m_segHdr.target_actor = bothActorId;
   };

protected:
   /** Initializes the fields of the header structures */
   virtual void initialize( void )
   {
      // Zero out the structures.
      memset( &m_segHdr, 0, sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) );
      memset( &m_ipcHdr, 0, sizeof( FFXIVARR_IPC_HEADER ) );
      memset( &m_data, 0, sizeof( T ) );

      // Set the values of static fields.
      // The size must be the sum of the segment header, the ipc header, and
      // the IPC data itself.
      m_segHdr.size = sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) + sizeof( FFXIVARR_IPC_HEADER ) + sizeof( T );
      // Game packets (IPC) are type 3.
      m_segHdr.type = 3;
      // The IPC type itself.
      m_ipcHdr.type = static_cast< ServerZoneIpcType >( m_data._ServerIpcType );
   };

public:
   virtual T1 ipcType()
   {
      return static_cast< T1 >( m_data._ServerIpcType );
   };

   /** Gets a reference to the underlying IPC data structure. */
   T& data( void ) { return m_data; };

   /**
   * @brief Sets the source actor id for this IPC packet.
   * @param actorId The source actor id.
   * @return This IPC packet object (can be used for chaining).
   */
   GamePacketNew< T, T1 > sourceActor( uint32_t actorId )
   {
      m_segHdr.source_actor = actorId;
      return this;
   };

   /**
   * @brief Gets the source actor id for this IPC packet.
   * @return The source actor id.
   */
   uint32_t sourceActor( void ) const
   {
      return m_segHdr.source_actor;
   };

   /**
   * @brief Sets the target actor id for this IPC packet.
   * @param actorId The target actor id.
   * @return This IPC packet object (can be used for chaining).
   */
   GamePacketNew< T, T1 > targetActor( uint32_t actorId )
   {
      m_segHdr.target_actor = actorId;
      return this;
   };

   /**
   * @brief Gets the target actor id for this IPC packet.
   * @return The target actor id.
   */
   uint32_t targetActor( void ) const
   {
      return m_segHdr.target_actor;
   };

   friend std::ostream& operator<< <> ( std::ostream& os, const GamePacketNew< T, T1 >& packet );

   friend class GamePacketFactory;

   /**
   * @brief Adapts the new-style packet into the old style.
   */
   operator GamePacket* ( ) const
   {
      std::ostringstream buf;
      serialize( buf );
      // NOTE: This should be ok because CGamePacket's constructor will
      // copy the contents of the buffer.
      GamePacket* pOldStyle = new GamePacket( const_cast< char* >( buf.str().c_str() ), m_segHdr.size, false );
      return pOldStyle;
   };

   operator GamePacketPtr () const
   {
      std::ostringstream buf;
      serialize( buf );
      // NOTE: This should be ok because CGamePacket's constructor will
      // copy the contents of the buffer.
      GamePacketPtr pOldStyle( new GamePacket( const_cast< char* >( buf.str().c_str() ), m_segHdr.size, true ) );
      return pOldStyle;
   };

protected:
   // TODO: Is this a waste of storage memory?
   /** The segment header */
   FFXIVARR_PACKET_SEGMENT_HEADER m_segHdr;
   /** The IPC packet header */
   FFXIVARR_IPC_HEADER m_ipcHdr;
   /** The underlying data portion of the packet as a structure */
   T m_data;

private:
   std::ostream& serialize( std::ostream& os ) const
   {
      // Since the packet itself is constant, we need to make a copy of the IPC
      // header in order to set the timestamp.
      FFXIVARR_IPC_HEADER ipcHdr;
      memcpy( &ipcHdr, &m_ipcHdr, sizeof( ipcHdr ) );

      // TODO: Fixed timestamp? Can we use a manipulator on the stream to assign
      // a fixed timestamp value. This might be useful if several packets must
      // be sent having the exact same timestamp. (Maybe this doesn't really
      // need to happen though...)
      ipcHdr.timestamp = static_cast< uint32_t >( time( nullptr ) );

      // TODO: What about encryption? compression?
      // Ideally, these could come directly from the stream using manipulators.
      // We could check the stream's flags, and perform the appropriate
      // operations here. The snag is encryption, which does not occur for
      // segment headers, but may occur for IPC headers, and their data.
      // Compression occurs for the entire segment header down.
      os << m_segHdr << ipcHdr;
      return os.write( reinterpret_cast< const char* >( &m_data ), sizeof( T ) );
   };
};

template < typename T, typename T1 >
std::ostream& operator<<( std::ostream& os, const GamePacketNew<T, T1>& packet )
{
#if 0
   // Since the packet itself is constant, we need to make a copy of the IPC
   // header in order to set the timestamp.
   FFXIVARR_IPC_HEADER ipcHdr;
   memcpy( &ipcHdr, &packet.m_ipcHdr, sizeof( ipcHdr ) );

   // TODO: Fixed timestamp? Can we use a manipulator on the stream to assign
   // a fixed timestamp value. This might be useful if several packets must
   // be sent having the exact same timestamp. (Maybe this doesn't really
   // need to happen though...)
   ipcHdr.timestamp = time( NULL );

   // TODO: What about encryption? compression?
   // Ideally, these could come directly from the stream using manipulators.
   // We could check the stream's flags, and perform the appropriate
   // operations here. The snag is encryption, which does not occur for
   // segment headers, but may occur for IPC headers, and their data.
   // Compression occurs for the entire segment header down.
   os << packet.m_segHdr << ipcHdr;
   return os.write(
      reinterpret_cast< const char* >( &packet.m_data ), sizeof( T ) );
#else
   return packet.serialize( os );
#endif
}


////////////////////////////////////////////////7

class FFXIVPacketBase
{
public:
   FFXIVPacketBase() :
     m_segmentType( 0 )
   {
      initializeSegmentHeader();
   }

   FFXIVPacketBase( uint16_t segmentType, uint32_t sourceActorId, uint32_t targetActorId ) :
      m_segmentType( segmentType )
   {
      initializeSegmentHeader();
      setSourceActor( sourceActorId );
      setTargetActor( targetActorId );
   }

   std::size_t getSize() const
   {
      return m_segHdr.size;
   }

   virtual std::vector< uint8_t > getData()
   {
      return {};
   }

protected:
   /** The segment header */
   FFXIVARR_PACKET_SEGMENT_HEADER m_segHdr;
   uint16_t m_segmentType;

protected:
   virtual uint32_t getContentSize() { return 0; };
   virtual std::vector< uint8_t > getContent() { return{}; };

   /**
   * @brief Gets the segment type of this packet.
   */
   uint16_t getSegmentType() const
   {
      return m_segmentType;
   }

   /**
   * @brief Sets the source actor id for this packet.
   * @param actorId The source actor id.
   */
   void setSourceActor( uint32_t actorId )
   {
      m_segHdr.source_actor = actorId;
   };

   /**
   * @brief Gets the source actor id for this packet.
   * @return The source actor id.
   */
   uint32_t getSourceActor() const
   {
      return m_segHdr.source_actor;
   };

   /**
   * @brief Sets the target actor id for this packet.
   * @param actorId The target actor id.
   */
   void setTargetActor( uint32_t actorId )
   {
      m_segHdr.target_actor = actorId;
   };

   /**
   * @brief Gets the target actor id for this packet.
   */
   uint32_t getTargetActor( void ) const
   {
      return m_segHdr.target_actor;
   };

   /** Initializes the fields of the segment header structure */
   virtual void initializeSegmentHeader( void )
   {
      // Zero out the structure.
      memset( &m_segHdr, 0, sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) );

      // Set the values of static fields.
      // The size must be the sum of the segment header and the content
      m_segHdr.size = sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) + getContentSize();
      m_segHdr.type = getSegmentType();
   }

};

template < typename T, typename T1 >
class FFXIVIpcPacket : public FFXIVIpcPacketBase< T1 >, public FFXIVPacketBase
{
public:
   FFXIVIpcPacket< T, T1 >( uint32_t sourceActorId, uint32_t targetActorId ) :
      FFXIVPacketBase( 3, sourceActorId, targetActorId )
   {
      initialize();
   };

   FFXIVIpcPacket< T, T1 >( uint32_t sourceActorId ) :
      FFXIVPacketBase( 3, sourceActorId, sourceActorId )
   {
      initialize();
   };

   uint32_t getContentSize() override
   {
      return sizeof( FFXIVARR_IPC_HEADER ) + sizeof( T );
   }

   std::vector< uint8_t > getContent() override
   {
      std::vector< uint8_t > content( getContentSize() );
      memcpy( content.data(), &m_ipcHdr, sizeof( FFXIVARR_IPC_HEADER ) );
      memcpy( content.data() + sizeof( FFXIVARR_IPC_HEADER ), &m_data, sizeof( T ) );
      return content;
   }

   std::vector< uint8_t > getData() override
   {
      std::vector< uint8_t > data( sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) + sizeof( FFXIVARR_IPC_HEADER ) + sizeof( m_data ) );

      memcpy( &data[0], &m_segHdr, sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) );
      memcpy( &data[sizeof( FFXIVARR_PACKET_SEGMENT_HEADER )], &m_ipcHdr, sizeof( FFXIVARR_IPC_HEADER ) );
      memcpy( &data[sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) + sizeof( FFXIVARR_IPC_HEADER )], &m_data, sizeof( m_data ) );

      return data;
   }

   virtual T1 ipcType()
   {
      return static_cast< T1 >( m_data._ServerIpcType );
   };

   /** Gets a reference to the underlying IPC data structure. */
   T& data() { return m_data; };

protected:
   /** Initializes the fields of the header structures */
   virtual void initialize()
   {
      // Zero out the structures.
      memset( &m_ipcHdr, 0, sizeof( FFXIVARR_IPC_HEADER ) );
      memset( &m_data, 0, sizeof( T ) );

      // The IPC type itself.
      m_ipcHdr.type = static_cast< ServerZoneIpcType >( m_data._ServerIpcType );
   };

protected:
   /** The IPC packet header */
   FFXIVARR_IPC_HEADER m_ipcHdr;
   /** The underlying data portion of the packet as a structure */
   T m_data;
};


class FFXIVRawPacket : public FFXIVPacketBase
{
public:
   FFXIVRawPacket( uint16_t type, uint32_t size, uint32_t sourceActorId, uint32_t targetActorId ) :
      FFXIVPacketBase( type, sourceActorId, targetActorId )
   {
      m_data.resize( size - sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) );
      initialize();
      m_segHdr.size = size;
   };

   uint32_t getContentSize() override
   {
      return m_data.size();
   }

   std::vector< uint8_t > getContent() override
   {
      return m_data;
   }

   std::vector< uint8_t > getData() override
   {
      std::vector< uint8_t > data( sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) + m_data.size() );

      memcpy( &data[0], &m_segHdr, sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) );
      memcpy( &data[sizeof( FFXIVARR_PACKET_SEGMENT_HEADER )], &m_data[0], m_data.size() );

      return data;
   }

   /** Gets a reference to the underlying IPC data structure. */
   std::vector< uint8_t >& data() { return m_data; };

protected:
   /** Initializes the fields of the header structures */
   virtual void initialize()
   {
      // Zero out the structures.
      memset( &m_data[0], 0, m_data.size() - sizeof( FFXIVARR_PACKET_SEGMENT_HEADER ) );
   };

protected:
   /** The underlying data portion of the packet as a structure */
   std::vector< uint8_t > m_data;
};


} /* Packets */
} /* Network */
} /* Core */

#endif /*_CORE_NETWORK_PACKETS_CGAMEPACKETNEW_H*/