#include "Component.h"

namespace Kazia {

Component::Component() {
    // 生成 UUID
    char uuid_str[37];
    #ifdef _WIN32
        RPC_STATUS status;
        UUID uuid;
        status = UuidCreate(&uuid);
        if (status == RPC_S_OK || status == RPC_S_UUID_LOCAL_ONLY)
        {
            unsigned char* uuid_str_ptr;
            status = UuidToStringA(&uuid, &uuid_str_ptr);
            if (status == RPC_S_OK)
            {
                strcpy_s(uuid_str, sizeof(uuid_str), (char*)uuid_str_ptr);
                RpcStringFreeA(&uuid_str_ptr);
            }
        }
    #else
        uuid_t uuid;
        uuid_generate(uuid);
        uuid_unparse(uuid, uuid_str);
    #endif
    m_uuid = uuid_str;
    
    m_owner = nullptr;
}

} // namespace Kazia
