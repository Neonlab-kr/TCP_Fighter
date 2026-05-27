namespace Generated.RPC
{
    public interface IC2S_Sender
    {
        bool SendRPCPacket(byte[] packet);
    }

    public sealed class C2S_Proxy
    {
        private readonly IC2S_Sender _sender;

        public C2S_Proxy(IC2S_Sender sender)
        {
            _sender = sender;
        }

        public bool MoveStart(byte direction, short x, short y)
        {
            byte[] buffer = new byte[RPCCommon.HeaderSize + RPCCommon.PayloadSize_C2S_MOVE_START];
            int offset = 0;
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PacketCode);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PayloadSize_C2S_MOVE_START);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.C2S_MOVE_START);
            RPCCommon.WriteUInt8(buffer, ref offset, direction);
            RPCCommon.WriteInt16(buffer, ref offset, x);
            RPCCommon.WriteInt16(buffer, ref offset, y);
            return _sender != null && _sender.SendRPCPacket(buffer);
        }

        public bool MoveStop(byte direction, short x, short y)
        {
            byte[] buffer = new byte[RPCCommon.HeaderSize + RPCCommon.PayloadSize_C2S_MOVE_STOP];
            int offset = 0;
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PacketCode);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PayloadSize_C2S_MOVE_STOP);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.C2S_MOVE_STOP);
            RPCCommon.WriteUInt8(buffer, ref offset, direction);
            RPCCommon.WriteInt16(buffer, ref offset, x);
            RPCCommon.WriteInt16(buffer, ref offset, y);
            return _sender != null && _sender.SendRPCPacket(buffer);
        }

        public bool Attack1(byte direction, short x, short y)
        {
            byte[] buffer = new byte[RPCCommon.HeaderSize + RPCCommon.PayloadSize_C2S_ATTACK1];
            int offset = 0;
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PacketCode);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PayloadSize_C2S_ATTACK1);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.C2S_ATTACK1);
            RPCCommon.WriteUInt8(buffer, ref offset, direction);
            RPCCommon.WriteInt16(buffer, ref offset, x);
            RPCCommon.WriteInt16(buffer, ref offset, y);
            return _sender != null && _sender.SendRPCPacket(buffer);
        }

        public bool Attack2(byte direction, short x, short y)
        {
            byte[] buffer = new byte[RPCCommon.HeaderSize + RPCCommon.PayloadSize_C2S_ATTACK2];
            int offset = 0;
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PacketCode);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PayloadSize_C2S_ATTACK2);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.C2S_ATTACK2);
            RPCCommon.WriteUInt8(buffer, ref offset, direction);
            RPCCommon.WriteInt16(buffer, ref offset, x);
            RPCCommon.WriteInt16(buffer, ref offset, y);
            return _sender != null && _sender.SendRPCPacket(buffer);
        }

        public bool Attack3(byte direction, short x, short y)
        {
            byte[] buffer = new byte[RPCCommon.HeaderSize + RPCCommon.PayloadSize_C2S_ATTACK3];
            int offset = 0;
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PacketCode);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PayloadSize_C2S_ATTACK3);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.C2S_ATTACK3);
            RPCCommon.WriteUInt8(buffer, ref offset, direction);
            RPCCommon.WriteInt16(buffer, ref offset, x);
            RPCCommon.WriteInt16(buffer, ref offset, y);
            return _sender != null && _sender.SendRPCPacket(buffer);
        }

        public bool Sync(short x, short y)
        {
            byte[] buffer = new byte[RPCCommon.HeaderSize + RPCCommon.PayloadSize_C2S_SYNC];
            int offset = 0;
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PacketCode);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.PayloadSize_C2S_SYNC);
            RPCCommon.WriteUInt8(buffer, ref offset, RPCCommon.C2S_SYNC);
            RPCCommon.WriteInt16(buffer, ref offset, x);
            RPCCommon.WriteInt16(buffer, ref offset, y);
            return _sender != null && _sender.SendRPCPacket(buffer);
        }

    }
}
