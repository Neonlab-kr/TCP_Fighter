using System;

namespace Generated
{
    public static class PacketProtocol
    {
        public const byte PacketCode = 0x89;
        public const int HeaderSize = 3;
        public const int MaxPayloadSize = 255;
        public const int MaxPacketSize = HeaderSize + MaxPayloadSize;

        public const byte SC_CREATE_MY_CHARACTER = 0;
        public const byte SC_CREATE_OTHER_CHARACTER = 1;
        public const byte SC_DELETE_CHARACTER = 2;
        public const byte CS_MOVE_START = 10;
        public const byte SC_MOVE_START = 11;
        public const byte CS_MOVE_STOP = 12;
        public const byte SC_MOVE_STOP = 13;
        public const byte CS_ATTACK1 = 20;
        public const byte SC_ATTACK1 = 21;
        public const byte CS_ATTACK2 = 22;
        public const byte SC_ATTACK2 = 23;
        public const byte CS_ATTACK3 = 24;
        public const byte SC_ATTACK3 = 25;
        public const byte SC_DAMAGE = 30;
        public const byte CS_SYNC = 250;
        public const byte SC_SYNC = 251;

        public const byte PayloadSize_SC_CREATE_MY_CHARACTER = 10;
        public const byte PayloadSize_SC_CREATE_OTHER_CHARACTER = 10;
        public const byte PayloadSize_SC_DELETE_CHARACTER = 4;
        public const byte PayloadSize_CS_MOVE_START = 5;
        public const byte PayloadSize_SC_MOVE_START = 9;
        public const byte PayloadSize_CS_MOVE_STOP = 5;
        public const byte PayloadSize_SC_MOVE_STOP = 9;
        public const byte PayloadSize_CS_ATTACK1 = 5;
        public const byte PayloadSize_SC_ATTACK1 = 9;
        public const byte PayloadSize_CS_ATTACK2 = 5;
        public const byte PayloadSize_SC_ATTACK2 = 9;
        public const byte PayloadSize_CS_ATTACK3 = 5;
        public const byte PayloadSize_SC_ATTACK3 = 9;
        public const byte PayloadSize_SC_DAMAGE = 9;
        public const byte PayloadSize_CS_SYNC = 4;
        public const byte PayloadSize_SC_SYNC = 8;

        public static byte[] MakePacket_SC_CREATE_MY_CHARACTER(int id, byte direction, short x, short y, byte hp)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_CREATE_MY_CHARACTER];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_CREATE_MY_CHARACTER);
            WriteUInt8(buffer, ref offset, SC_CREATE_MY_CHARACTER);
            WriteInt32(buffer, ref offset, id);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            WriteUInt8(buffer, ref offset, hp);
            return buffer;
        }

        public static bool ReadPayload_SC_CREATE_MY_CHARACTER(byte[] buffer, int offset, out int id, out byte direction, out short x, out short y, out byte hp)
        {
            id = default(int);
            direction = default(byte);
            x = default(short);
            y = default(short);
            hp = default(byte);
            try
            {
                id = ReadInt32(buffer, ref offset);
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                hp = ReadUInt8(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_SC_CREATE_OTHER_CHARACTER(int id, byte direction, short x, short y, byte hp)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_CREATE_OTHER_CHARACTER];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_CREATE_OTHER_CHARACTER);
            WriteUInt8(buffer, ref offset, SC_CREATE_OTHER_CHARACTER);
            WriteInt32(buffer, ref offset, id);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            WriteUInt8(buffer, ref offset, hp);
            return buffer;
        }

        public static bool ReadPayload_SC_CREATE_OTHER_CHARACTER(byte[] buffer, int offset, out int id, out byte direction, out short x, out short y, out byte hp)
        {
            id = default(int);
            direction = default(byte);
            x = default(short);
            y = default(short);
            hp = default(byte);
            try
            {
                id = ReadInt32(buffer, ref offset);
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                hp = ReadUInt8(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_SC_DELETE_CHARACTER(int id)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_DELETE_CHARACTER];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_DELETE_CHARACTER);
            WriteUInt8(buffer, ref offset, SC_DELETE_CHARACTER);
            WriteInt32(buffer, ref offset, id);
            return buffer;
        }

        public static bool ReadPayload_SC_DELETE_CHARACTER(byte[] buffer, int offset, out int id)
        {
            id = default(int);
            try
            {
                id = ReadInt32(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_CS_MOVE_START(byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_CS_MOVE_START];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_CS_MOVE_START);
            WriteUInt8(buffer, ref offset, CS_MOVE_START);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_CS_MOVE_START(byte[] buffer, int offset, out byte direction, out short x, out short y)
        {
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_SC_MOVE_START(int id, byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_MOVE_START];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_MOVE_START);
            WriteUInt8(buffer, ref offset, SC_MOVE_START);
            WriteInt32(buffer, ref offset, id);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_SC_MOVE_START(byte[] buffer, int offset, out int id, out byte direction, out short x, out short y)
        {
            id = default(int);
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                id = ReadInt32(buffer, ref offset);
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_CS_MOVE_STOP(byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_CS_MOVE_STOP];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_CS_MOVE_STOP);
            WriteUInt8(buffer, ref offset, CS_MOVE_STOP);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_CS_MOVE_STOP(byte[] buffer, int offset, out byte direction, out short x, out short y)
        {
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_SC_MOVE_STOP(int id, byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_MOVE_STOP];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_MOVE_STOP);
            WriteUInt8(buffer, ref offset, SC_MOVE_STOP);
            WriteInt32(buffer, ref offset, id);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_SC_MOVE_STOP(byte[] buffer, int offset, out int id, out byte direction, out short x, out short y)
        {
            id = default(int);
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                id = ReadInt32(buffer, ref offset);
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_CS_ATTACK1(byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_CS_ATTACK1];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_CS_ATTACK1);
            WriteUInt8(buffer, ref offset, CS_ATTACK1);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_CS_ATTACK1(byte[] buffer, int offset, out byte direction, out short x, out short y)
        {
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_SC_ATTACK1(int id, byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_ATTACK1];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_ATTACK1);
            WriteUInt8(buffer, ref offset, SC_ATTACK1);
            WriteInt32(buffer, ref offset, id);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_SC_ATTACK1(byte[] buffer, int offset, out int id, out byte direction, out short x, out short y)
        {
            id = default(int);
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                id = ReadInt32(buffer, ref offset);
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_CS_ATTACK2(byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_CS_ATTACK2];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_CS_ATTACK2);
            WriteUInt8(buffer, ref offset, CS_ATTACK2);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_CS_ATTACK2(byte[] buffer, int offset, out byte direction, out short x, out short y)
        {
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_SC_ATTACK2(int id, byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_ATTACK2];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_ATTACK2);
            WriteUInt8(buffer, ref offset, SC_ATTACK2);
            WriteInt32(buffer, ref offset, id);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_SC_ATTACK2(byte[] buffer, int offset, out int id, out byte direction, out short x, out short y)
        {
            id = default(int);
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                id = ReadInt32(buffer, ref offset);
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_CS_ATTACK3(byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_CS_ATTACK3];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_CS_ATTACK3);
            WriteUInt8(buffer, ref offset, CS_ATTACK3);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_CS_ATTACK3(byte[] buffer, int offset, out byte direction, out short x, out short y)
        {
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_SC_ATTACK3(int id, byte direction, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_ATTACK3];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_ATTACK3);
            WriteUInt8(buffer, ref offset, SC_ATTACK3);
            WriteInt32(buffer, ref offset, id);
            WriteUInt8(buffer, ref offset, direction);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_SC_ATTACK3(byte[] buffer, int offset, out int id, out byte direction, out short x, out short y)
        {
            id = default(int);
            direction = default(byte);
            x = default(short);
            y = default(short);
            try
            {
                id = ReadInt32(buffer, ref offset);
                direction = ReadUInt8(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_SC_DAMAGE(int attackId, int damageId, byte damageHp)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_DAMAGE];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_DAMAGE);
            WriteUInt8(buffer, ref offset, SC_DAMAGE);
            WriteInt32(buffer, ref offset, attackId);
            WriteInt32(buffer, ref offset, damageId);
            WriteUInt8(buffer, ref offset, damageHp);
            return buffer;
        }

        public static bool ReadPayload_SC_DAMAGE(byte[] buffer, int offset, out int attackId, out int damageId, out byte damageHp)
        {
            attackId = default(int);
            damageId = default(int);
            damageHp = default(byte);
            try
            {
                attackId = ReadInt32(buffer, ref offset);
                damageId = ReadInt32(buffer, ref offset);
                damageHp = ReadUInt8(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_CS_SYNC(short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_CS_SYNC];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_CS_SYNC);
            WriteUInt8(buffer, ref offset, CS_SYNC);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_CS_SYNC(byte[] buffer, int offset, out short x, out short y)
        {
            x = default(short);
            y = default(short);
            try
            {
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public static byte[] MakePacket_SC_SYNC(int id, short x, short y)
        {
            byte[] buffer = new byte[HeaderSize + PayloadSize_SC_SYNC];
            int offset = 0;
            WriteUInt8(buffer, ref offset, PacketCode);
            WriteUInt8(buffer, ref offset, PayloadSize_SC_SYNC);
            WriteUInt8(buffer, ref offset, SC_SYNC);
            WriteInt32(buffer, ref offset, id);
            WriteInt16(buffer, ref offset, x);
            WriteInt16(buffer, ref offset, y);
            return buffer;
        }

        public static bool ReadPayload_SC_SYNC(byte[] buffer, int offset, out int id, out short x, out short y)
        {
            id = default(int);
            x = default(short);
            y = default(short);
            try
            {
                id = ReadInt32(buffer, ref offset);
                x = ReadInt16(buffer, ref offset);
                y = ReadInt16(buffer, ref offset);
                return true;
            }
            catch
            {
                return false;
            }
        }

        private static void Require(byte[] buffer, int offset, int size)
        {
            if (buffer == null || offset < 0 || size < 0 || offset + size > buffer.Length)
                throw new ArgumentOutOfRangeException();
        }

        private static void WriteInt8(byte[] buffer, ref int offset, sbyte value) { Require(buffer, offset, 1); buffer[offset++] = unchecked((byte)value); }
        private static void WriteUInt8(byte[] buffer, ref int offset, byte value) { Require(buffer, offset, 1); buffer[offset++] = value; }
        private static void WriteInt16(byte[] buffer, ref int offset, short value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        private static void WriteUInt16(byte[] buffer, ref int offset, ushort value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        private static void WriteInt32(byte[] buffer, ref int offset, int value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        private static void WriteUInt32(byte[] buffer, ref int offset, uint value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        private static void WriteInt64(byte[] buffer, ref int offset, long value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        private static void WriteUInt64(byte[] buffer, ref int offset, ulong value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        private static void WriteFloat(byte[] buffer, ref int offset, float value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        private static void WriteDouble(byte[] buffer, ref int offset, double value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }

        private static sbyte ReadInt8(byte[] buffer, ref int offset) { Require(buffer, offset, 1); return unchecked((sbyte)buffer[offset++]); }
        private static byte ReadUInt8(byte[] buffer, ref int offset) { Require(buffer, offset, 1); return buffer[offset++]; }
        private static short ReadInt16(byte[] buffer, ref int offset) { Require(buffer, offset, 2); short value = BitConverter.ToInt16(buffer, offset); offset += 2; return value; }
        private static ushort ReadUInt16(byte[] buffer, ref int offset) { Require(buffer, offset, 2); ushort value = BitConverter.ToUInt16(buffer, offset); offset += 2; return value; }
        private static int ReadInt32(byte[] buffer, ref int offset) { Require(buffer, offset, 4); int value = BitConverter.ToInt32(buffer, offset); offset += 4; return value; }
        private static uint ReadUInt32(byte[] buffer, ref int offset) { Require(buffer, offset, 4); uint value = BitConverter.ToUInt32(buffer, offset); offset += 4; return value; }
        private static long ReadInt64(byte[] buffer, ref int offset) { Require(buffer, offset, 8); long value = BitConverter.ToInt64(buffer, offset); offset += 8; return value; }
        private static ulong ReadUInt64(byte[] buffer, ref int offset) { Require(buffer, offset, 8); ulong value = BitConverter.ToUInt64(buffer, offset); offset += 8; return value; }
        private static float ReadFloat(byte[] buffer, ref int offset) { Require(buffer, offset, 4); float value = BitConverter.ToSingle(buffer, offset); offset += 4; return value; }
        private static double ReadDouble(byte[] buffer, ref int offset) { Require(buffer, offset, 8); double value = BitConverter.ToDouble(buffer, offset); offset += 8; return value; }

        private static void WriteBytes(byte[] buffer, ref int offset, byte[] value)
        {
            Require(buffer, offset, value.Length);
            Buffer.BlockCopy(value, 0, buffer, offset, value.Length);
            offset += value.Length;
        }
    }
}
