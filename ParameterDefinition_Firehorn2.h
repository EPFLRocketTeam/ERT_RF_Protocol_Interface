// LoRa Parameters Definition of the ERT2023 network architecture

// Default RF parameters
#define DEFAULT_TX_POWER            20  // dBm   max 20dBm
#define DEFAULT_BW                  125E3  // Hz
#define DEFAULT_SF                  8          
#define DEFAULT_CR                  7

#define DEFAULT_PREAMBLE_LENGTH     8       
#define DEFAULT_CRC                 true   
#define DEFAULT_SYNC_WORD           0x34

// -------- UPLINK -------- // 
#define UPLINK_FREQUENCY            864.34E6
#define UPLINK_POWER                DEFAULT_TX_POWER
#define UPLINK_BW                   DEFAULT_BW
#define UPLINK_SF                   DEFAULT_SF
#define UPLINK_CR                   DEFAULT_CR
#define UPLINK_PREAMBLE_LEN         DEFAULT_PREAMBLE_LENGTH
#define UPLINK_CRC                  DEFAULT_CRC
#define UPLINK_INVERSE_IQ           false
#define UPLINK_SYNC_WORD            DEFAULT_SYNC_WORD

// -------- AV_DOWNLINK -------- // 
#define AV_DOWNLINK_FREQUENCY       866.34E6
#define AV_DOWNLINK_POWER           DEFAULT_TX_POWER
#define AV_DOWNLINK_BW              250E3
#define AV_DOWNLINK_SF              7
#define AV_DOWNLINK_CR              3
#define AV_DOWNLINK_PREAMBLE_LEN    DEFAULT_PREAMBLE_LENGTH
#define AV_DOWNLINK_CRC             DEFAULT_CRC
#define AV_DOWNLINK_INVERSE_IQ      false
#define AV_DOWNLINK_SYNC_WORD       DEFAULT_SYNC_WORD

// -------- GSE_DOWNLINK -------- // 
#define GSE_DOWNLINK_FREQUENCY      868.67E6
#define GSE_DOWNLINK_POWER          DEFAULT_TX_POWER
#define GSE_DOWNLINK_BW             DEFAULT_BW
#define GSE_DOWNLINK_SF             DEFAULT_SF
#define GSE_DOWNLINK_CR             DEFAULT_CR
#define GSE_DOWNLINK_PREAMBLE_LEN   DEFAULT_PREAMBLE_LENGTH
#define GSE_DOWNLINK_CRC            DEFAULT_CRC
#define GSE_DOWNLINK_INVERSE_IQ     false
#define GSE_DOWNLINK_SYNC_WORD      DEFAULT_SYNC_WORD


