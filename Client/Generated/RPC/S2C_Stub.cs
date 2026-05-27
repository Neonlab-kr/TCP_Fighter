namespace Generated.RPC
{
    public abstract class S2C_Stub
    {
        public bool Dispatch(byte[] buffer, int offset, byte packetType)
        {
            switch (packetType)
            {
                case RPCCommon.S2C_CREATE_MY_CHARACTER:
                {
                    int id;
                    byte direction;
                    short x;
                    short y;
                    byte hp;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                        direction = RPCCommon.ReadUInt8(buffer, ref offset);
                        x = RPCCommon.ReadInt16(buffer, ref offset);
                        y = RPCCommon.ReadInt16(buffer, ref offset);
                        hp = RPCCommon.ReadUInt8(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnCreateMyCharacter(id, direction, x, y, hp);
                }
                case RPCCommon.S2C_CREATE_OTHER_CHARACTER:
                {
                    int id;
                    byte direction;
                    short x;
                    short y;
                    byte hp;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                        direction = RPCCommon.ReadUInt8(buffer, ref offset);
                        x = RPCCommon.ReadInt16(buffer, ref offset);
                        y = RPCCommon.ReadInt16(buffer, ref offset);
                        hp = RPCCommon.ReadUInt8(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnCreateOtherCharacter(id, direction, x, y, hp);
                }
                case RPCCommon.S2C_DELETE_CHARACTER:
                {
                    int id;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnDeleteCharacter(id);
                }
                case RPCCommon.S2C_MOVE_START:
                {
                    int id;
                    byte direction;
                    short x;
                    short y;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                        direction = RPCCommon.ReadUInt8(buffer, ref offset);
                        x = RPCCommon.ReadInt16(buffer, ref offset);
                        y = RPCCommon.ReadInt16(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnMoveStart(id, direction, x, y);
                }
                case RPCCommon.S2C_MOVE_STOP:
                {
                    int id;
                    byte direction;
                    short x;
                    short y;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                        direction = RPCCommon.ReadUInt8(buffer, ref offset);
                        x = RPCCommon.ReadInt16(buffer, ref offset);
                        y = RPCCommon.ReadInt16(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnMoveStop(id, direction, x, y);
                }
                case RPCCommon.S2C_ATTACK1:
                {
                    int id;
                    byte direction;
                    short x;
                    short y;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                        direction = RPCCommon.ReadUInt8(buffer, ref offset);
                        x = RPCCommon.ReadInt16(buffer, ref offset);
                        y = RPCCommon.ReadInt16(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnAttack1(id, direction, x, y);
                }
                case RPCCommon.S2C_ATTACK2:
                {
                    int id;
                    byte direction;
                    short x;
                    short y;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                        direction = RPCCommon.ReadUInt8(buffer, ref offset);
                        x = RPCCommon.ReadInt16(buffer, ref offset);
                        y = RPCCommon.ReadInt16(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnAttack2(id, direction, x, y);
                }
                case RPCCommon.S2C_ATTACK3:
                {
                    int id;
                    byte direction;
                    short x;
                    short y;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                        direction = RPCCommon.ReadUInt8(buffer, ref offset);
                        x = RPCCommon.ReadInt16(buffer, ref offset);
                        y = RPCCommon.ReadInt16(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnAttack3(id, direction, x, y);
                }
                case RPCCommon.S2C_DAMAGE:
                {
                    int id;
                    int damageId;
                    byte damageHp;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                        damageId = RPCCommon.ReadInt32(buffer, ref offset);
                        damageHp = RPCCommon.ReadUInt8(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnDamage(id, damageId, damageHp);
                }
                case RPCCommon.S2C_SYNC:
                {
                    int id;
                    short x;
                    short y;
                    try
                    {
                        id = RPCCommon.ReadInt32(buffer, ref offset);
                        x = RPCCommon.ReadInt16(buffer, ref offset);
                        y = RPCCommon.ReadInt16(buffer, ref offset);
                    }
                    catch
                    {
                        return false;
                    }
                    return OnSync(id, x, y);
                }
                default:
                    return false;
            }
        }

        protected abstract bool OnCreateMyCharacter(int id, byte direction, short x, short y, byte hp);
        protected abstract bool OnCreateOtherCharacter(int id, byte direction, short x, short y, byte hp);
        protected abstract bool OnDeleteCharacter(int id);
        protected abstract bool OnMoveStart(int id, byte direction, short x, short y);
        protected abstract bool OnMoveStop(int id, byte direction, short x, short y);
        protected abstract bool OnAttack1(int id, byte direction, short x, short y);
        protected abstract bool OnAttack2(int id, byte direction, short x, short y);
        protected abstract bool OnAttack3(int id, byte direction, short x, short y);
        protected abstract bool OnDamage(int id, int damageId, byte damageHp);
        protected abstract bool OnSync(int id, short x, short y);
    }
}
