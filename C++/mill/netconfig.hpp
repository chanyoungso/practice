#ifndef NETCONFIG_HPP
#define NETCONFIG_HPP

#ifdef DEBUG
#define SERVER_PORT ""
#define CLIENT_PORT ""
#endif

// Hex Command in protocol (1.byte: command, 2.byte: # of bytes to read after that)
#define HEX_GROUP                       static_cast<quint8>(0x04)

#define HEX_ASK_CON                     static_cast<quint8>(0x01)
#define HEX_LEN_ASK_CON                 static_cast<quint8>(0x00)

#define HEX_GAME                        static_cast<quint8>(0x02)
#define HEX_LEN_GAME                    static_cast<quint8>(0x02)
#define HEX_GAME_S                      static_cast<quint8>(0x00)
#define HEX_GAME_C                      static_cast<quint8>(0x01)
#define HEX_GAME_R                      static_cast<quint8>(0x02)

#define HEX_OK                          static_cast<quint8>(0x02)
#define HEX_LEN_OK                      static_cast<quint8>(0x01)

#define HEX_PLACE                       static_cast<quint8>(0x10)
#define HEX_LEN_PLACE                   static_cast<quint8>(0x04)

#define HEX_MOVE                        static_cast<quint8>(0x11)
#define HEX_LEN_MOVE                    static_cast<quint8>(0x04)

#define HEX_JUMP                        static_cast<quint8>(0x12)
#define HEX_LEN_JUMP                    static_cast<quint8>(0x05)

#define HEX_RES                         static_cast<quint8>(0x13)
#define HEX_LEN_RES                     static_cast<quint8>(0x01)
#define HEX_RES_TURN_OVER               static_cast<quint8>(0x00)
#define HEX_RES_MUEHLE                  static_cast<quint8>(0x01)
#define HEX_RES_ERR_NO_STONE            static_cast<quint8>(0x10)
#define HEX_RES_ERR_POS_OCCU            static_cast<quint8>(0x11)
#define HEX_RES_ERR_INV_MOVE            static_cast<quint8>(0x12)
#define HEX_RES_ERR_NOT_NOW             static_cast<quint8>(0x13)

#define HEX_YES                         static_cast<quint8>(0x01)
#define HEX_NO                          static_cast<quint8>(0x00)

#define HEX_REMOVE                      static_cast<quint8>(0x20)
#define HEX_LEN_REMOVE                  static_cast<quint8>(0x02)

#define HEX_RES_RMV                     static_cast<quint8>(0x21)
#define HEX_LEN_RES_RMV                 static_cast<quint8>(0x02)
#define HEX_RES_RMV_TURN_OVER           static_cast<quint8>(0x00)
#define HEX_RES_RMV_GAME_OVER           static_cast<quint8>(0x01)
#define HEX_RES_RMV_ERR_MUH_STONE       static_cast<quint8>(0x10)
#define HEX_RES_RMV_ERR_NO_STONE        static_cast<quint8>(0x11)
#define HEX_RES_RMV_ERR_NO_REASON       static_cast<quint8>(0x12)

#define HEX_DIR_NORTH                   static_cast<quint8>(0x01)
#define HEX_DIR_SOUTH                   static_cast<quint8>(0x03)
#define HEX_DIR_EAST                    static_cast<quint8>(0x02)
#define HEX_DIR_WEST                    static_cast<quint8>(0x04)

#endif // NETCONFIG_HPP
