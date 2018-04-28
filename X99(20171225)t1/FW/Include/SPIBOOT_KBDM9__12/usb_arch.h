#ifndef __MUSBC_ARCH_H__
#define __MUSBC_ARCH_H__

//#include <usb_def.h>

#define BIG_ENDIAN 1
/**
 * Read an 8-bit register from the core
 * @param _base_addr core base address in memory
 * @return 8-bit datum
 */
#define MUSBC_Read8( _base_addr ) *((volatile uint8_t *)(_base_addr))

/**
 * Read an 16-bit register from the core
 * @param _base_addr core base address in memory
 * @return 16-bit datum
 */
#define MUSBC_Read16( _base_addr ) *((volatile uint16_t *)(_base_addr))

/**
 * Read an 32-bit register from the core
 * @param _base_addr core base address in memory
 * @return 32-bit datum
 */
#define MUSBC_Read32( _base_addr ) *((volatile uint32_t *)(_base_addr))


/**
 * Write an 8-bit core register
 * @param _base_addr core base address in memory
 * @param _data 8-bit datum
 */
#define MUSBC_Write8(_base_addr, _data) \
	(*((volatile uint8_t *)(_base_addr)) = _data)

/**
 * Write an 16-bit core register
 * @param _base_addr core base address in memory
 * @param _data 16-bit datum
 */
#define MUSBC_Write16(_base_addr, _data) \
	(*((volatile uint16_t *)(_base_addr)) = _data)
	
/**
 * Write an 32-bit core register
 * @param _base_addr core base address in memory
 * @param _data 32-bit datum
 */
#define MUSBC_Write32(_base_addr, _data) \
	(*((volatile uint32_t *)(_base_addr)) = _data)

#ifdef BIG_ENDIAN

/**
 * Swap the given 16-bit quantity from USB byte order (little-endian) 
 * to host byte order.
 * @param _data the 16-bit quantity
 * @return quantity in host byte order
 */
#define MUSBC_SWAP16(_data) (((_data) << 8) | ((_data) >> 8))

/**
 * Swap the given 32-bit quantity from USB byte order (little-endian) 
 * to host byte order.
 * @param _data the 32-bit quantity
 * @return quantity in host byte order
 */
#define MUSBC_SWAP32(_data) ( ((_data) << 24) | ((_data) << 16) | ((_data) >> 24) | ((_data) >> 16) )

#else	// Little Endian

#define MUSBC_SWAP16(_data) (_data) 

#define MUSBC_SWAP32(_data) (_data) 

#endif	// Endian Difference

/**
 * Swap the pointed-to 16-bit quantity from USB byte order (little-endian) 
 * to host byte order, in an alignment-safe manner.
 * @param _pData pointer (as uint8_t*)
 * @return quantity in host byte order
 */
#define MUSBC_SWAP16P(_pData) ((*(_pData+1) << 8) | *(_pData))

/**
 * Swap the pointed-to 24-bit quantity from USB byte order (little-endian) 
 * to host byte order, in an alignment-safe manner.
 * @param _pData pointer (as uint8_t*)
 * @return quantity in host byte order
 */
#define MUSBC_SWAP24P(_pData) ((*(_pData+2) << 16) | (*(_pData+1) << 8) | *(_pData))

/**
 * Swap the pointed-to 32-bit quantity from USB byte order (little-endian) 
 * to host byte order, in an alignment-safe manner.
 * @param _pData pointer (as uint8_t*)
 * @return quantity in host byte order
 */
#define MUSBC_SWAP32P(_pData) ((*(_pData+3) << 24) | (*(_pData+2) << 16) | (*(_pData+1) << 8) | *(_pData))


#endif 	/* multiple inclusion protection */