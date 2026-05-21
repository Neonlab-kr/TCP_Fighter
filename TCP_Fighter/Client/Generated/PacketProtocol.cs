using System;
using System.Collections.Generic;

namespace Generated
{
    public static class PacketProtocol
    {
        public const byte PacketCode = 137;
        public const int HeaderSize = 3;
        public const int MaxPayloadSize = 255;

        public const byte CS_MOVE_START = 10;
        public const byte CS_MOVE_STOP = 11;
        public const byte CS_ATTACK1 = 12;
        public const byte CS_ATTACK2 = 13;
        public const byte CS_ATTACK3 = 14;
        public const byte CS_SYNC = 15;
        public const byte SC_CREATE_MY_CHARACTER = 20;
        public const byte SC_CREATE_OTHER_CHARACTER = 21;
        public const byte SC_DELETE_CHARACTER = 22;
        public const byte SC_MOVE_START = 23;
        public const byte SC_MOVE_STOP = 24;
        public const byte SC_ATTACK1 = 25;
        public const byte SC_ATTACK2 = 26;
        public const byte SC_ATTACK3 = 27;
        public const byte SC_DAMAGE = 28;
        public const byte SC_SYNC = 29;

        public const byte PayloadSize_CS_MOVE_START = 5;
        public const byte PayloadSize_CS_MOVE_STOP = 5;
        public const byte PayloadSize_CS_ATTACK1 = 5;
        public const byte PayloadSize_CS_ATTACK2 = 5;
        public const byte PayloadSize_CS_ATTACK3 = 5;
        public const byte PayloadSize_CS_SYNC = 4;
        public const byte PayloadSize_SC_CREATE_MY_CHARACTER = 10;
        public const byte PayloadSize_SC_CREATE_OTHER_CHARACTER = 10;
        public const byte PayloadSize_SC_DELETE_CHARACTER = 4;
        public const byte PayloadSize_SC_MOVE_START = 9;
        public const byte PayloadSize_SC_MOVE_STOP = 9;
        public const byte PayloadSize_SC_ATTACK1 = 9;
        public const byte PayloadSize_SC_ATTACK2 = 9;
        public const byte PayloadSize_SC_ATTACK3 = 9;
        public const byte PayloadSize_SC_DAMAGE = 9;
        public const byte PayloadSize_SC_SYNC = 8;

        public static byte[] MakePacket_CS_MOVE_START(byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_CS_MOVE_START);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_CS_MOVE_START);
            WriteUInt8(buffer, CS_MOVE_START);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_CS_MOVE_STOP(byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_CS_MOVE_STOP);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_CS_MOVE_STOP);
            WriteUInt8(buffer, CS_MOVE_STOP);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_CS_ATTACK1(byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_CS_ATTACK1);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_CS_ATTACK1);
            WriteUInt8(buffer, CS_ATTACK1);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_CS_ATTACK2(byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_CS_ATTACK2);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_CS_ATTACK2);
            WriteUInt8(buffer, CS_ATTACK2);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_CS_ATTACK3(byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_CS_ATTACK3);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_CS_ATTACK3);
            WriteUInt8(buffer, CS_ATTACK3);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_CS_SYNC(short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_CS_SYNC);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_CS_SYNC);
            WriteUInt8(buffer, CS_SYNC);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_CREATE_MY_CHARACTER(int id, byte direction, short x, short y, byte hp)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_CREATE_MY_CHARACTER);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_CREATE_MY_CHARACTER);
            WriteUInt8(buffer, SC_CREATE_MY_CHARACTER);
            WriteInt32(buffer, id);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            WriteUInt8(buffer, hp);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_CREATE_OTHER_CHARACTER(int id, byte direction, short x, short y, byte hp)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_CREATE_OTHER_CHARACTER);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_CREATE_OTHER_CHARACTER);
            WriteUInt8(buffer, SC_CREATE_OTHER_CHARACTER);
            WriteInt32(buffer, id);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            WriteUInt8(buffer, hp);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_DELETE_CHARACTER(int id)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_DELETE_CHARACTER);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_DELETE_CHARACTER);
            WriteUInt8(buffer, SC_DELETE_CHARACTER);
            WriteInt32(buffer, id);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_MOVE_START(int id, byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_MOVE_START);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_MOVE_START);
            WriteUInt8(buffer, SC_MOVE_START);
            WriteInt32(buffer, id);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_MOVE_STOP(int id, byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_MOVE_STOP);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_MOVE_STOP);
            WriteUInt8(buffer, SC_MOVE_STOP);
            WriteInt32(buffer, id);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_ATTACK1(int id, byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_ATTACK1);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_ATTACK1);
            WriteUInt8(buffer, SC_ATTACK1);
            WriteInt32(buffer, id);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_ATTACK2(int id, byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_ATTACK2);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_ATTACK2);
            WriteUInt8(buffer, SC_ATTACK2);
            WriteInt32(buffer, id);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_ATTACK3(int id, byte direction, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_ATTACK3);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_ATTACK3);
            WriteUInt8(buffer, SC_ATTACK3);
            WriteInt32(buffer, id);
            WriteUInt8(buffer, direction);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_DAMAGE(int attackId, int damageId, byte damageHp)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_DAMAGE);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_DAMAGE);
            WriteUInt8(buffer, SC_DAMAGE);
            WriteInt32(buffer, attackId);
            WriteInt32(buffer, damageId);
            WriteUInt8(buffer, damageHp);
            return buffer.ToArray();
        }

        public static byte[] MakePacket_SC_SYNC(int id, short x, short y)
        {
            List<byte> buffer = new List<byte>(HeaderSize + PayloadSize_SC_SYNC);
            WriteUInt8(buffer, PacketCode);
            WriteUInt8(buffer, PayloadSize_SC_SYNC);
            WriteUInt8(buffer, SC_SYNC);
            WriteInt32(buffer, id);
            WriteInt16(buffer, x);
            WriteInt16(buffer, y);
            return buffer.ToArray();
        }

        private static void WriteInt8(List<byte> buffer, sbyte value) { buffer.Add(unchecked((byte)value)); }
        private static void WriteUInt8(List<byte> buffer, byte value) { buffer.Add(value); }
        private static void WriteInt16(List<byte> buffer, short value) { buffer.AddRange(BitConverter.GetBytes(value)); }
        private static void WriteUInt16(List<byte> buffer, ushort value) { buffer.AddRange(BitConverter.GetBytes(value)); }
        private static void WriteInt32(List<byte> buffer, int value) { buffer.AddRange(BitConverter.GetBytes(value)); }
        private static void WriteUInt32(List<byte> buffer, uint value) { buffer.AddRange(BitConverter.GetBytes(value)); }
        private static void WriteInt64(List<byte> buffer, long value) { buffer.AddRange(BitConverter.GetBytes(value)); }
        private static void WriteUInt64(List<byte> buffer, ulong value) { buffer.AddRange(BitConverter.GetBytes(value)); }
        private static void WriteFloat(List<byte> buffer, float value) { buffer.AddRange(BitConverter.GetBytes(value)); }
        private static void WriteDouble(List<byte> buffer, double value) { buffer.AddRange(BitConverter.GetBytes(value)); }

        private static sbyte ReadInt8(byte[] buffer, ref int offset) { sbyte value = unchecked((sbyte)buffer[offset]); offset += 1; return value; }
        private static byte ReadUInt8(byte[] buffer, ref int offset) { byte value = buffer[offset]; offset += 1; return value; }
        private static short ReadInt16(byte[] buffer, ref int offset) { short value = BitConverter.ToInt16(buffer, offset); offset += 2; return value; }
        private static ushort ReadUInt16(byte[] buffer, ref int offset) { ushort value = BitConverter.ToUInt16(buffer, offset); offset += 2; return value; }
        private static int ReadInt32(byte[] buffer, ref int offset) { int value = BitConverter.ToInt32(buffer, offset); offset += 4; return value; }
        private static uint ReadUInt32(byte[] buffer, ref int offset) { uint value = BitConverter.ToUInt32(buffer, offset); offset += 4; return value; }
        private static long ReadInt64(byte[] buffer, ref int offset) { long value = BitConverter.ToInt64(buffer, offset); offset += 8; return value; }
        private static ulong ReadUInt64(byte[] buffer, ref int offset) { ulong value = BitConverter.ToUInt64(buffer, offset); offset += 8; return value; }
        private static float ReadFloat(byte[] buffer, ref int offset) { float value = BitConverter.ToSingle(buffer, offset); offset += 4; return value; }
        private static double ReadDouble(byte[] buffer, ref int offset) { double value = BitConverter.ToDouble(buffer, offset); offset += 8; return value; }
    }
}
