using System;

namespace Generated.RPC
{
    public static class RPCCommon
    {
        public const byte PacketCode = 0x89;
        public const int HeaderSize = 3;
        public const int MaxPayloadSize = 255;
        public const int MaxPacketSize = HeaderSize + MaxPayloadSize;

        public const byte PACKET_MOVE_DIR_LL = 0;
        public const byte PACKET_MOVE_DIR_LU = 1;
        public const byte PACKET_MOVE_DIR_UU = 2;
        public const byte PACKET_MOVE_DIR_RU = 3;
        public const byte PACKET_MOVE_DIR_RR = 4;
        public const byte PACKET_MOVE_DIR_RD = 5;
        public const byte PACKET_MOVE_DIR_DD = 6;
        public const byte PACKET_MOVE_DIR_LD = 7;

        public const byte S2C_CREATE_MY_CHARACTER = 0;
        public const byte S2C_CREATE_OTHER_CHARACTER = 1;
        public const byte S2C_DELETE_CHARACTER = 2;
        public const byte C2S_MOVE_START = 10;
        public const byte S2C_MOVE_START = 11;
        public const byte C2S_MOVE_STOP = 12;
        public const byte S2C_MOVE_STOP = 13;
        public const byte C2S_ATTACK1 = 20;
        public const byte S2C_ATTACK1 = 21;
        public const byte C2S_ATTACK2 = 22;
        public const byte S2C_ATTACK2 = 23;
        public const byte C2S_ATTACK3 = 24;
        public const byte S2C_ATTACK3 = 25;
        public const byte S2C_DAMAGE = 30;
        public const byte C2S_SYNC = 250;
        public const byte S2C_SYNC = 251;

        public const byte PayloadSize_S2C_CREATE_MY_CHARACTER = 10;
        public const byte PayloadSize_S2C_CREATE_OTHER_CHARACTER = 10;
        public const byte PayloadSize_S2C_DELETE_CHARACTER = 4;
        public const byte PayloadSize_C2S_MOVE_START = 5;
        public const byte PayloadSize_S2C_MOVE_START = 9;
        public const byte PayloadSize_C2S_MOVE_STOP = 5;
        public const byte PayloadSize_S2C_MOVE_STOP = 9;
        public const byte PayloadSize_C2S_ATTACK1 = 5;
        public const byte PayloadSize_S2C_ATTACK1 = 9;
        public const byte PayloadSize_C2S_ATTACK2 = 5;
        public const byte PayloadSize_S2C_ATTACK2 = 9;
        public const byte PayloadSize_C2S_ATTACK3 = 5;
        public const byte PayloadSize_S2C_ATTACK3 = 9;
        public const byte PayloadSize_S2C_DAMAGE = 9;
        public const byte PayloadSize_C2S_SYNC = 4;
        public const byte PayloadSize_S2C_SYNC = 8;

        public static void Require(byte[] buffer, int offset, int size)
        {
            if (buffer == null || offset < 0 || size < 0 || offset + size > buffer.Length)
                throw new ArgumentOutOfRangeException();
        }

        public static void WriteInt8(byte[] buffer, ref int offset, sbyte value) { Require(buffer, offset, 1); buffer[offset++] = unchecked((byte)value); }
        public static void WriteUInt8(byte[] buffer, ref int offset, byte value) { Require(buffer, offset, 1); buffer[offset++] = value; }
        public static void WriteInt16(byte[] buffer, ref int offset, short value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        public static void WriteUInt16(byte[] buffer, ref int offset, ushort value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        public static void WriteInt32(byte[] buffer, ref int offset, int value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        public static void WriteUInt32(byte[] buffer, ref int offset, uint value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        public static void WriteInt64(byte[] buffer, ref int offset, long value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        public static void WriteUInt64(byte[] buffer, ref int offset, ulong value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        public static void WriteFloat(byte[] buffer, ref int offset, float value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }
        public static void WriteDouble(byte[] buffer, ref int offset, double value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }

        public static sbyte ReadInt8(byte[] buffer, ref int offset) { Require(buffer, offset, 1); return unchecked((sbyte)buffer[offset++]); }
        public static byte ReadUInt8(byte[] buffer, ref int offset) { Require(buffer, offset, 1); return buffer[offset++]; }
        public static short ReadInt16(byte[] buffer, ref int offset) { Require(buffer, offset, 2); short value = BitConverter.ToInt16(buffer, offset); offset += 2; return value; }
        public static ushort ReadUInt16(byte[] buffer, ref int offset) { Require(buffer, offset, 2); ushort value = BitConverter.ToUInt16(buffer, offset); offset += 2; return value; }
        public static int ReadInt32(byte[] buffer, ref int offset) { Require(buffer, offset, 4); int value = BitConverter.ToInt32(buffer, offset); offset += 4; return value; }
        public static uint ReadUInt32(byte[] buffer, ref int offset) { Require(buffer, offset, 4); uint value = BitConverter.ToUInt32(buffer, offset); offset += 4; return value; }
        public static long ReadInt64(byte[] buffer, ref int offset) { Require(buffer, offset, 8); long value = BitConverter.ToInt64(buffer, offset); offset += 8; return value; }
        public static ulong ReadUInt64(byte[] buffer, ref int offset) { Require(buffer, offset, 8); ulong value = BitConverter.ToUInt64(buffer, offset); offset += 8; return value; }
        public static float ReadFloat(byte[] buffer, ref int offset) { Require(buffer, offset, 4); float value = BitConverter.ToSingle(buffer, offset); offset += 4; return value; }
        public static double ReadDouble(byte[] buffer, ref int offset) { Require(buffer, offset, 8); double value = BitConverter.ToDouble(buffer, offset); offset += 8; return value; }

        private static void WriteBytes(byte[] buffer, ref int offset, byte[] value)
        {
            Require(buffer, offset, value.Length);
            Buffer.BlockCopy(value, 0, buffer, offset, value.Length);
            offset += value.Length;
        }
    }
}
