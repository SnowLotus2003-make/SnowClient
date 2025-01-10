//
// Created by vastrakai on 6/28/2024.
//

#pragma once

enum class PacketID : int {
    Unknown                           = -1,
    Login                             = 0x1,
    PlayStatus                        = 0x2,
    ServerToClientHandshake           = 0x3,
    ClientToServerHandshake           = 0x4,
    Disconnect                        = 0x5,
    ResourcePacksInfo                 = 0x6,
    ResourcePackStack                 = 0x7,
    ResourcePackClientResponse        = 0x8,
    Text                              = 0x9,
    SetTime                           = 0xA,
    StartGame                         = 0xB,
    AddPlayer                         = 0xC,
    AddActor                          = 0xD,
    RemoveActor                       = 0xE,
    AddItemActor                      = 0xF,
    TakeItemActor                     = 0x11,
    MoveActorAbsolute                 = 0x12,
    MovePlayer                        = 0x13,
    PassengerJump                     = 0x14,
    UpdateBlock                       = 0x15,
    AddPainting                       = 0x16,
    LevelSoundEventV1                 = 0x18,
    LevelEvent                        = 0x19,
    BlockEvent                        = 0x1A,
    ActorEvent                        = 0x1B,
    MobEffect                         = 0x1C,
    UpdateAttributes                  = 0x1D,
    InventoryTransaction              = 0x1E,
    MobEquipment                      = 0x1F,
    MobArmorEquipment                 = 0x20,
    Interact                          = 0x21,
    BlockPickRequest                  = 0x22,
    ActorPickRequest                  = 0x23,
    PlayerAction                      = 0x24,
    HurtArmor                         = 0x26,
    SetActorData                      = 0x27,
    SetActorMotion                    = 0x28,
    SetActorLink                      = 0x29,
    SetHealth                         = 0x2A,
    SetSpawnPosition                  = 0x2B,
    Animate                           = 0x2C,
    Respawn                           = 0x2D,
    ContainerOpen                     = 0x2E,
    ContainerClose                    = 0x2F,
    PlayerHotbar                      = 0x30,
    InventoryContent                  = 0x31,
    InventorySlot                     = 0x32,
    ContainerSetData                  = 0x33,
    CraftingData                      = 0x34,
    GuiDataPickItem                   = 0x36,
    BlockActorData                    = 0x38,
    PlayerInput                       = 0x39,
    LevelChunk                        = 0x3A, // FullChunkData
    SetCommandsEnabled                = 0x3B,
    SetDifficulty                     = 0x3C,
    ChangeDimension                   = 0x3D,
    SetPlayerGameType                 = 0x3E,
    PlayerList                        = 0x3F,
    SimpleEvent                       = 0x40,
    LegacyTelemetryEvent              = 0x41,
    SpawnExperienceOrb                = 0x42,
    ClientboundMapItemData            = 0x43,
    MapInfoRequest                    = 0x44,
    RequestChunkRadius                = 0x45,
    ChunkRadiusUpdated                = 0x46,
    GameRulesChanged                  = 0x48,
    Camera                            = 0x49,
    BossEvent                         = 0x4A,
    ShowCredits                       = 0x4B,
    AvailableCommands                 = 0x4C,
    CommandRequest                    = 0x4D,
    CommandBlockUpdate                = 0x4E,
    CommandOutput                     = 0x4F,
    UpdateTrade                       = 0x50,
    UpdateEquip                       = 0x51,
    ResourcePackDataInfo              = 0x52,
    ResourcePackChunkData             = 0x53,
    ResourcePackChunkRequest          = 0x54,
    Transfer                          = 0x55,
    PlaySound                         = 0x56,
    StopSound                         = 0x57,
    SetTitle                          = 0x58,
    AddBehaviorTree                   = 0x59,
    StructureBlockUpdate              = 0x5A,
    ShowStoreOffer                    = 0x5B,
    PurchaseReceipt                   = 0x5C,
    PlayerSkin                        = 0x5D,
    SubClientLogin                    = 0x5E,
    AutomationClientConnect           = 0x5F,
    SetLastHurtBy                     = 0x60,
    BookEdit                          = 0x61,
    NpcRequest                        = 0x62,
    PhotoTransfer                     = 0x63,
    ModalFormRequest                  = 0x64,
    ModalFormResponse                 = 0x65,
    ServerSettingsRequest             = 0x66,
    ServerSettingsResponse            = 0x67,
    ShowProfile                       = 0x68,
    SetDefaultGameType                = 0x69,
    RemoveObjective                   = 0x6A,
    SetDisplayObjective               = 0x6B,
    SetScore                          = 0x6C,
    LabTable                          = 0x6D,
    UpdateBlockSynced                 = 0x6E,
    MoveActorDelta                    = 0x6F,
    SetScoreboardIdentity             = 0x70,
    SetLocalPlayerAsInit              = 0x71,
    UpdateSoftEnum                    = 0x72,
    NetworkStackLatency               = 0x73, // Ping
    SpawnParticleEffect               = 0x76,
    AvailableActorIdentifiers         = 0x77,
    LevelSoundEventV2                 = 0x78,
    NetworkChunkPublisherUpdate       = 0x79,
    BiomeDefinitionList               = 0x7A,
    LevelSoundEvent                   = 0x7B,
    LevelEventGeneric                 = 0x7C,
    LecternUpdate                     = 0x7D,
    Unk7E                             = 0x7E,
    Unk7F                             = 0x7F,
    RemoveEntity_Deprecated           = 0x80,
    ClientCacheStatus                 = 0x81,
    OnScreenTextureAnimation          = 0x82,
    MapCreateLockedCopy               = 0x83,
    StructureTemplateDataRequest      = 0x84,
    StructureTemplateDataResponse     = 0x85,
    UNUSED_PLS_USE_ME2                = 0x86,
    ClientCacheBlobStatus             = 0x87,
    ClientCacheMissResponse           = 0x88,
    EducationSettings                 = 0x89,
    Emote                             = 0x8A,
    MultiplayerSettings               = 0x8B,
    SettingsCommand                   = 0x8C,
    AnvilDamage                       = 0x8D,
    CompletedUsingItem                = 0x8E,
    NetworkSettings                   = 0x8F,
    PlayerAuthInput                   = 0x90,
    CreativeContent                   = 0x91,
    PlayerEnchantOptions              = 0x92,
    ItemStackRequest                  = 0x93,
    ItemStackResponse                 = 0x94,
    PlayerArmorDamage                 = 0x95,
    CodeBuilder                       = 0x96,
    UpdatePlayerGameType              = 0x97,
    EmoteList                         = 0x98,
    PositionTrackingDBServerBroadcast = 0x99,
    PositionTrackingDBClientRequest   = 0x9A,
    DebugInfo                         = 0x9B,
    PacketViolationWarning            = 0x9C,
    MotionPredictionHints             = 0x9D,
    AnimateEntity                     = 0x9E, // TriggerAnimation
    CameraShake                       = 0x9F,
    PlayerFog                         = 0xA0, // PlayerFogSetting
    CorrectPlayerMovePrediction       = 0xA1,
    ItemComponent                     = 0xA2,
    FilterText_Deprecated             = 0xA3,
    ClientboundDebugRenderer          = 0xA4,
    SyncActorProperty                 = 0xA5,
    AddVolumeEntity                   = 0xA6,
    RemoveVolumeEntity                = 0xA7,
    SimulationType                    = 0xA8,
    NpcDialogue                       = 0xA9,
    EduUriResource                    = 0xAA,
    CreatePhoto                       = 0xAB,
    UpdateSubChunkBlocks              = 0xAC,
    PhotoInfoRequest_Deprecated       = 0xAD,
    SubChunk                          = 0xAE,
    SubChunkRequest                   = 0xAF,
    PlayerStartItemCooldown           = 0xB0,
    ScriptMessage                     = 0xB1,
    CodeBuilderSource                 = 0xB2,
    TickingAreasLoadStatus            = 0xB3,
    DimensionData                     = 0xB4,
    AgentAction                       = 0xB5,
    ChangeMobProperty                 = 0xB6,
    LessonProgress                    = 0xB7,
    RequestAbility                    = 0xB8,
    RequestPermissions                = 0xB9,
    ToastRequest                      = 0xBA,
    UpdateAbilities                   = 0xBB,
    UpdateAdventureSettings           = 0xBC,
    DeathInfo                         = 0xBD,
    EditorNetwork                     = 0xBE,
    FeatureRegistry                   = 0xBF,
    ServerStats                       = 0xC0,
    RequestNetworkSettings            = 0xC1,
    GameTestRequest                   = 0xC2,
    GameTestResults                   = 0xC3,
    UpdateClientInputLocks            = 0xC4, // PlayerClientInputPermissions
    ClientCheatAbility_Deprecated     = 0xC5,
    CameraPresets                     = 0xC6,
    UnlockedRecipes                   = 0xC7,
    CameraInstruction                 = 0x12C,
    CompressedBiomeDefinitionList     = 0x12D,
    TrimData                          = 0x12E,
    OpenSign                          = 0x12F,
    AgentAnimation                    = 0x130,
    RefreshEntitlements               = 0x131,
    PlayerToggleCrafterSlotRequest    = 0x132,
    SetPlayerInventoryOptions         = 0x133,
    SetHud                            = 0x134,
    AwardAchievementPacket            = 0x135,
    EndId                             = 0x136,
};