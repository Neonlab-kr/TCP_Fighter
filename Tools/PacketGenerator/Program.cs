using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;

namespace PacketGenerator
{
    internal sealed class ProtocolSettings
    {
        public int PacketCode = -1;
        public int HeaderSize = -1;
        public int MaxPayloadSize = -1;

        public string PacketCodeHex
        {
            get { return "0x" + PacketCode.ToString("X2", CultureInfo.InvariantCulture); }
        }
    }

    internal sealed class TypeInfo
    {
        public readonly string CppType;
        public readonly string CsType;
        public readonly string CsWrite;
        public readonly string CsRead;
        public readonly int Size;

        public TypeInfo(string cppType, string csType, string csWrite, string csRead, int size)
        {
            CppType = cppType;
            CsType = csType;
            CsWrite = csWrite;
            CsRead = csRead;
            Size = size;
        }
    }

    internal sealed class FieldDef
    {
        public string TypeName;
        public string Name;
        public TypeInfo Type;
    }

    internal sealed class PacketDef
    {
        public string Direction;
        public string Name;
        public int Id;
        public readonly List<FieldDef> Fields = new List<FieldDef>();

        public string DirectionUpper { get { return Direction.ToUpperInvariant(); } }
        public string NameUpper { get { return TextUtil.ToSnakeUpper(Name); } }
        public string Symbol { get { return DirectionUpper + "_" + NameUpper; } }

        public int PayloadSize
        {
            get
            {
                int size = 0;
                for (int i = 0; i < Fields.Count; ++i)
                    size += Fields[i].Type.Size;
                return size;
            }
        }
    }

    internal sealed class ProtocolDef
    {
        public readonly ProtocolSettings Settings = new ProtocolSettings();
        public readonly List<PacketDef> Packets = new List<PacketDef>();
    }

    internal static class Program
    {
        private const string ServerGameRelativePath = "TCP_FIGHTER/Game";
        private const string ClientGeneratedRelativePath = "Client/Generated";
        private const string ProtocolRelativePath = "Tools/PacketGenerator/Protocol/GameProtocol.rpc";

        private static readonly Dictionary<string, TypeInfo> Types = CreateTypeMap();

        private static int Main(string[] args)
        {
            try
            {
                string root = PathUtil.FindRoot(ProtocolRelativePath);
                string protocolPath = PathUtil.Combine(root, ProtocolRelativePath);
                string serverGamePath = PathUtil.Combine(root, ServerGameRelativePath);
                string clientGeneratedPath = PathUtil.Combine(root, ClientGeneratedRelativePath);

                string cppDefinePath = Path.Combine(serverGamePath, "PacketDefine.h");
                string cppMakerPath = Path.Combine(serverGamePath, "PacketMaker.h");
                string csProtocolPath = Path.Combine(clientGeneratedPath, "PacketProtocol.cs");

                ProtocolDef protocol = ProtocolParser.Parse(protocolPath, Types);
                ProtocolValidator.Validate(protocol);

                PathUtil.EnsureParentDirectory(cppDefinePath);
                PathUtil.EnsureParentDirectory(cppMakerPath);
                PathUtil.EnsureParentDirectory(csProtocolPath);

                File.WriteAllText(cppDefinePath, CppGenerator.GeneratePacketDefine(protocol), new UTF8Encoding(false));
                File.WriteAllText(cppMakerPath, CppGenerator.GeneratePacketMaker(protocol), new UTF8Encoding(false));
                File.WriteAllText(csProtocolPath, CSharpGenerator.Generate(protocol), new UTF8Encoding(false));

                Console.WriteLine("PacketGenerator completed.");
                Console.WriteLine("Protocol     : " + protocolPath);
                Console.WriteLine("PacketDefine : " + cppDefinePath);
                Console.WriteLine("PacketMaker  : " + cppMakerPath);
                Console.WriteLine("C# Protocol  : " + csProtocolPath);
                Console.WriteLine("Packet Count : " + protocol.Packets.Count.ToString(CultureInfo.InvariantCulture));
                return 0;
            }
            catch (Exception ex)
            {
                Console.WriteLine("[PacketGenerator Error]");
                Console.WriteLine(ex.Message);
                return 1;
            }
        }

        private static Dictionary<string, TypeInfo> CreateTypeMap()
        {
            Dictionary<string, TypeInfo> map = new Dictionary<string, TypeInfo>(StringComparer.Ordinal);
            map.Add("int8", new TypeInfo("std::int8_t", "sbyte", "WriteInt8", "ReadInt8", 1));
            map.Add("uint8", new TypeInfo("std::uint8_t", "byte", "WriteUInt8", "ReadUInt8", 1));
            map.Add("int16", new TypeInfo("std::int16_t", "short", "WriteInt16", "ReadInt16", 2));
            map.Add("uint16", new TypeInfo("std::uint16_t", "ushort", "WriteUInt16", "ReadUInt16", 2));
            map.Add("int32", new TypeInfo("std::int32_t", "int", "WriteInt32", "ReadInt32", 4));
            map.Add("uint32", new TypeInfo("std::uint32_t", "uint", "WriteUInt32", "ReadUInt32", 4));
            map.Add("int64", new TypeInfo("std::int64_t", "long", "WriteInt64", "ReadInt64", 8));
            map.Add("uint64", new TypeInfo("std::uint64_t", "ulong", "WriteUInt64", "ReadUInt64", 8));
            map.Add("float", new TypeInfo("float", "float", "WriteFloat", "ReadFloat", 4));
            map.Add("double", new TypeInfo("double", "double", "WriteDouble", "ReadDouble", 8));
            return map;
        }
    }

    internal static class ProtocolParser
    {
        public static ProtocolDef Parse(string path, Dictionary<string, TypeInfo> types)
        {
            if (!File.Exists(path))
                throw new FileNotFoundException("Protocol file not found.", path);

            ProtocolDef protocol = new ProtocolDef();
            string[] lines = File.ReadAllLines(path);

            for (int i = 0; i < lines.Length; ++i)
            {
                string line = TextUtil.TrimStatement(TextUtil.StripComment(lines[i]));
                if (line.Length == 0)
                    continue;

                if (IsSetting(line))
                    ParseSetting(protocol.Settings, line, i + 1);
                else
                    protocol.Packets.Add(ParsePacket(line, i + 1, types));
            }

            return protocol;
        }

        private static bool IsSetting(string line)
        {
            return line.IndexOf('=') > 0 && line.IndexOf('(') < 0;
        }

        private static void ParseSetting(ProtocolSettings settings, string line, int lineNumber)
        {
            int equal = line.IndexOf('=');
            string key = line.Substring(0, equal).Trim().ToLowerInvariant();
            int value = TextUtil.ParseIntLiteral(line.Substring(equal + 1).Trim(), lineNumber);

            if (key == "packet_code")
                settings.PacketCode = value;
            else if (key == "header_size")
                settings.HeaderSize = value;
            else if (key == "max_payload_size")
                settings.MaxPayloadSize = value;
            else
                throw new Exception("Line " + lineNumber + ": unknown setting: " + key);
        }

        private static PacketDef ParsePacket(string line, int lineNumber, Dictionary<string, TypeInfo> types)
        {
            int split = TextUtil.IndexOfWhiteSpace(line, 0);
            if (split <= 0)
                throw new Exception("Line " + lineNumber + ": expected 'cs Name = id(type name, ...)' or 'sc Name = id(...)'.");

            PacketDef packet = new PacketDef();
            packet.Direction = line.Substring(0, split).Trim().ToLowerInvariant();
            if (packet.Direction != "cs" && packet.Direction != "sc")
                throw new Exception("Line " + lineNumber + ": direction must be 'cs' or 'sc'.");

            string body = line.Substring(split).Trim();
            int equal = body.IndexOf('=');
            if (equal < 0)
                throw new Exception("Line " + lineNumber + ": packet id is required.");

            packet.Name = body.Substring(0, equal).Trim();
            if (!TextUtil.IsIdentifier(packet.Name))
                throw new Exception("Line " + lineNumber + ": invalid packet name: " + packet.Name);

            string right = body.Substring(equal + 1).Trim();
            int open = right.IndexOf('(');
            int close = right.LastIndexOf(')');
            if (open <= 0 || close < open)
                throw new Exception("Line " + lineNumber + ": invalid packet parameter list.");

            packet.Id = TextUtil.ParseIntLiteral(right.Substring(0, open).Trim(), lineNumber);
            if (packet.Id < 0 || packet.Id > 255)
                throw new Exception("Line " + lineNumber + ": packet id must be 0~255.");

            string args = right.Substring(open + 1, close - open - 1).Trim();
            if (args.Length > 0)
                ParseFields(packet, args, lineNumber, types);

            return packet;
        }

        private static void ParseFields(PacketDef packet, string args, int lineNumber, Dictionary<string, TypeInfo> types)
        {
            string[] fields = TextUtil.SplitByComma(args);
            for (int i = 0; i < fields.Length; ++i)
                packet.Fields.Add(ParseField(fields[i].Trim(), i, lineNumber, types));
        }

        private static FieldDef ParseField(string text, int index, int lineNumber, Dictionary<string, TypeInfo> types)
        {
            string[] parts = TextUtil.SplitByWhiteSpace(text);
            if (parts.Length == 0 || parts.Length > 2)
                throw new Exception("Line " + lineNumber + ": invalid field: " + text);

            TypeInfo type;
            if (!types.TryGetValue(parts[0], out type))
                throw new Exception("Line " + lineNumber + ": unsupported type: " + parts[0]);

            string name = parts.Length == 2 ? parts[1] : "arg" + index.ToString(CultureInfo.InvariantCulture);
            if (!TextUtil.IsIdentifier(name))
                throw new Exception("Line " + lineNumber + ": invalid field name: " + name);

            FieldDef field = new FieldDef();
            field.TypeName = parts[0];
            field.Name = name;
            field.Type = type;
            return field;
        }
    }

    internal static class ProtocolValidator
    {
        public static void Validate(ProtocolDef protocol)
        {
            ValidateSettings(protocol.Settings);

            Dictionary<int, string> ids = new Dictionary<int, string>();
            HashSet<string> symbols = new HashSet<string>(StringComparer.Ordinal);

            for (int i = 0; i < protocol.Packets.Count; ++i)
            {
                PacketDef packet = protocol.Packets[i];

                if (packet.PayloadSize > protocol.Settings.MaxPayloadSize)
                    throw new Exception(packet.Symbol + ": payload size exceeds max_payload_size.");

                string oldSymbol;
                if (ids.TryGetValue(packet.Id, out oldSymbol))
                    throw new Exception("Duplicate packet id " + packet.Id + ": " + oldSymbol + " / " + packet.Symbol);

                if (!symbols.Add(packet.Symbol))
                    throw new Exception("Duplicate packet symbol: " + packet.Symbol);

                ids.Add(packet.Id, packet.Symbol);
            }
        }

        private static void ValidateSettings(ProtocolSettings settings)
        {
            if (settings.PacketCode < 0 || settings.PacketCode > 255)
                throw new Exception("packet_code must be defined and in range 0~255.");

            if (settings.HeaderSize <= 0)
                throw new Exception("header_size must be defined and greater than 0.");

            if (settings.MaxPayloadSize <= 0 || settings.MaxPayloadSize > 255)
                throw new Exception("max_payload_size must be defined and in range 1~255.");
        }
    }

    internal static class CppGenerator
    {
        public static string GeneratePacketDefine(ProtocolDef protocol)
        {
            CodeWriter w = new CodeWriter();
            w.Line("#pragma once");
            w.Line();
            w.Line("#include <cstdint>");
            w.Line();
            w.Line("constexpr std::uint8_t dfPACKET_CODE = static_cast<std::uint8_t>(" + protocol.Settings.PacketCodeHex + ");");
            w.Line("constexpr int dfPACKET_HEADER_SIZE = " + protocol.Settings.HeaderSize.ToString(CultureInfo.InvariantCulture) + ";");
            w.Line("constexpr int dfPACKET_MAX_PAYLOAD_SIZE = " + protocol.Settings.MaxPayloadSize.ToString(CultureInfo.InvariantCulture) + ";");
            w.Line("constexpr int dfPACKET_MAX_SIZE = dfPACKET_HEADER_SIZE + dfPACKET_MAX_PAYLOAD_SIZE;");
            w.Line();
            AppendMoveDirections(w);
            w.Line();
            AppendPacketIds(w, protocol);
            w.Line();
            AppendPayloadSizes(w, protocol);
            return w.ToString();
        }

        public static string GeneratePacketMaker(ProtocolDef protocol)
        {
            CodeWriter w = new CodeWriter();
            w.Line("#pragma once");
            w.Line();
            w.Line("#include <cstdint>");
            w.Line("#include \"PacketDefine.h\"");
            w.Line("#include \"../Core/SerializationBuffer.h\"");
            w.Line();
            w.Line("inline void WritePacketHeader(CPacket& packet, std::uint8_t packetType, std::uint8_t payloadSize)");
            w.BeginBlock();
            w.Line("packet << dfPACKET_CODE;");
            w.Line("packet << payloadSize;");
            w.Line("packet << packetType;");
            w.EndBlock();
            w.Line();

            for (int i = 0; i < protocol.Packets.Count; ++i)
            {
                AppendMakePacket(w, protocol.Packets[i]);
                w.Line();
                AppendReadPayload(w, protocol.Packets[i]);
                w.Line();
            }

            return w.ToString();
        }

        private static void AppendMoveDirections(CodeWriter w)
        {
            w.Line("constexpr std::uint8_t dfPACKET_MOVE_DIR_LL = 0;");
            w.Line("constexpr std::uint8_t dfPACKET_MOVE_DIR_LU = 1;");
            w.Line("constexpr std::uint8_t dfPACKET_MOVE_DIR_UU = 2;");
            w.Line("constexpr std::uint8_t dfPACKET_MOVE_DIR_RU = 3;");
            w.Line("constexpr std::uint8_t dfPACKET_MOVE_DIR_RR = 4;");
            w.Line("constexpr std::uint8_t dfPACKET_MOVE_DIR_RD = 5;");
            w.Line("constexpr std::uint8_t dfPACKET_MOVE_DIR_DD = 6;");
            w.Line("constexpr std::uint8_t dfPACKET_MOVE_DIR_LD = 7;");
        }

        private static void AppendPacketIds(CodeWriter w, ProtocolDef protocol)
        {
            for (int i = 0; i < protocol.Packets.Count; ++i)
            {
                PacketDef p = protocol.Packets[i];
                w.Line("constexpr std::uint8_t dfPACKET_" + p.Symbol + " = static_cast<std::uint8_t>(" + p.Id.ToString(CultureInfo.InvariantCulture) + ");");
            }
        }

        private static void AppendPayloadSizes(CodeWriter w, ProtocolDef protocol)
        {
            for (int i = 0; i < protocol.Packets.Count; ++i)
            {
                PacketDef p = protocol.Packets[i];
                w.Line("constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_" + p.Symbol + " = static_cast<std::uint8_t>(" + p.PayloadSize.ToString(CultureInfo.InvariantCulture) + ");");
            }
        }

        private static void AppendMakePacket(CodeWriter w, PacketDef p)
        {
            w.Line("inline void MakePacket_" + p.Symbol + "(CPacket& packet" + CppParameterList(p, false) + ")");
            w.BeginBlock();
            w.Line("packet.Clear();");
            w.Line("WritePacketHeader(packet, dfPACKET_" + p.Symbol + ", dfPACKET_PAYLOAD_SIZE_" + p.Symbol + ");");
            for (int i = 0; i < p.Fields.Count; ++i)
                w.Line("packet << " + p.Fields[i].Name + ";");
            w.EndBlock();
        }

        private static void AppendReadPayload(CodeWriter w, PacketDef p)
        {
            w.Line("inline bool ReadPayload_" + p.Symbol + "(CPacket& packet" + CppParameterList(p, true) + ")");
            w.BeginBlock();
            for (int i = 0; i < p.Fields.Count; ++i)
                w.Line("packet >> " + p.Fields[i].Name + ";");
            w.Line("return !packet.IsError();");
            w.EndBlock();
        }

        private static string CppParameterList(PacketDef p, bool byRef)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < p.Fields.Count; ++i)
            {
                FieldDef f = p.Fields[i];
                sb.Append(", ");
                sb.Append(f.Type.CppType);
                if (byRef)
                    sb.Append("&");
                sb.Append(" ");
                sb.Append(f.Name);
            }
            return sb.ToString();
        }
    }

    internal static class CSharpGenerator
    {
        public static string Generate(ProtocolDef protocol)
        {
            CodeWriter w = new CodeWriter("    ");
            w.Line("using System;");
            w.Line();
            w.Line("namespace Generated");
            w.BeginBlock();
            w.Line("public static class PacketProtocol");
            w.BeginBlock();

            AppendConstants(w, protocol);
            AppendMakeAndReadFunctions(w, protocol);
            AppendPrimitiveIO(w);

            w.EndBlock();
            w.EndBlock();
            return w.ToString();
        }

        private static void AppendConstants(CodeWriter w, ProtocolDef protocol)
        {
            w.Line("public const byte PacketCode = " + protocol.Settings.PacketCodeHex + ";");
            w.Line("public const int HeaderSize = " + protocol.Settings.HeaderSize.ToString(CultureInfo.InvariantCulture) + ";");
            w.Line("public const int MaxPayloadSize = " + protocol.Settings.MaxPayloadSize.ToString(CultureInfo.InvariantCulture) + ";");
            w.Line("public const int MaxPacketSize = HeaderSize + MaxPayloadSize;");
            w.Line();

            for (int i = 0; i < protocol.Packets.Count; ++i)
                w.Line("public const byte " + protocol.Packets[i].Symbol + " = " + protocol.Packets[i].Id.ToString(CultureInfo.InvariantCulture) + ";");

            w.Line();

            for (int i = 0; i < protocol.Packets.Count; ++i)
                w.Line("public const byte PayloadSize_" + protocol.Packets[i].Symbol + " = " + protocol.Packets[i].PayloadSize.ToString(CultureInfo.InvariantCulture) + ";");

            w.Line();
        }

        private static void AppendMakeAndReadFunctions(CodeWriter w, ProtocolDef protocol)
        {
            for (int i = 0; i < protocol.Packets.Count; ++i)
            {
                AppendMakePacket(w, protocol.Packets[i]);
                w.Line();
                AppendReadPayload(w, protocol.Packets[i]);
                w.Line();
            }
        }

        private static void AppendMakePacket(CodeWriter w, PacketDef p)
        {
            w.Line("public static byte[] MakePacket_" + p.Symbol + "(" + CsParameterList(p, false) + ")");
            w.BeginBlock();
            w.Line("byte[] buffer = new byte[HeaderSize + PayloadSize_" + p.Symbol + "];");
            w.Line("int offset = 0;");
            w.Line("WriteUInt8(buffer, ref offset, PacketCode);");
            w.Line("WriteUInt8(buffer, ref offset, PayloadSize_" + p.Symbol + ");");
            w.Line("WriteUInt8(buffer, ref offset, " + p.Symbol + ");");
            for (int i = 0; i < p.Fields.Count; ++i)
            {
                FieldDef f = p.Fields[i];
                w.Line(f.Type.CsWrite + "(buffer, ref offset, " + f.Name + ");");
            }
            w.Line("return buffer;");
            w.EndBlock();
        }

        private static void AppendReadPayload(CodeWriter w, PacketDef p)
        {
            w.Line("public static bool ReadPayload_" + p.Symbol + "(byte[] buffer, int offset" + CsParameterList(p, true) + ")");
            w.BeginBlock();
            for (int i = 0; i < p.Fields.Count; ++i)
            {
                FieldDef f = p.Fields[i];
                w.Line(f.Name + " = default(" + f.Type.CsType + ");");
            }
            w.Line("try");
            w.BeginBlock();
            for (int i = 0; i < p.Fields.Count; ++i)
            {
                FieldDef f = p.Fields[i];
                w.Line(f.Name + " = " + f.Type.CsRead + "(buffer, ref offset);");
            }
            w.Line("return true;");
            w.EndBlock();
            w.Line("catch");
            w.BeginBlock();
            w.Line("return false;");
            w.EndBlock();
            w.EndBlock();
        }

        private static string CsParameterList(PacketDef p, bool isOut)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < p.Fields.Count; ++i)
            {
                if (i > 0 || isOut)
                    sb.Append(", ");
                FieldDef f = p.Fields[i];
                if (isOut)
                    sb.Append("out ");
                sb.Append(f.Type.CsType);
                sb.Append(" ");
                sb.Append(f.Name);
            }
            return sb.ToString();
        }

        private static void AppendPrimitiveIO(CodeWriter w)
        {
            w.Line("private static void Require(byte[] buffer, int offset, int size)");
            w.BeginBlock();
            w.Line("if (buffer == null || offset < 0 || size < 0 || offset + size > buffer.Length)");
            w.Indent();
            w.Line("throw new ArgumentOutOfRangeException();");
            w.Unindent();
            w.EndBlock();
            w.Line();

            w.Line("private static void WriteInt8(byte[] buffer, ref int offset, sbyte value) { Require(buffer, offset, 1); buffer[offset++] = unchecked((byte)value); }");
            w.Line("private static void WriteUInt8(byte[] buffer, ref int offset, byte value) { Require(buffer, offset, 1); buffer[offset++] = value; }");
            w.Line("private static void WriteInt16(byte[] buffer, ref int offset, short value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }");
            w.Line("private static void WriteUInt16(byte[] buffer, ref int offset, ushort value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }");
            w.Line("private static void WriteInt32(byte[] buffer, ref int offset, int value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }");
            w.Line("private static void WriteUInt32(byte[] buffer, ref int offset, uint value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }");
            w.Line("private static void WriteInt64(byte[] buffer, ref int offset, long value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }");
            w.Line("private static void WriteUInt64(byte[] buffer, ref int offset, ulong value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }");
            w.Line("private static void WriteFloat(byte[] buffer, ref int offset, float value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }");
            w.Line("private static void WriteDouble(byte[] buffer, ref int offset, double value) { WriteBytes(buffer, ref offset, BitConverter.GetBytes(value)); }");
            w.Line();
            w.Line("private static sbyte ReadInt8(byte[] buffer, ref int offset) { Require(buffer, offset, 1); return unchecked((sbyte)buffer[offset++]); }");
            w.Line("private static byte ReadUInt8(byte[] buffer, ref int offset) { Require(buffer, offset, 1); return buffer[offset++]; }");
            w.Line("private static short ReadInt16(byte[] buffer, ref int offset) { Require(buffer, offset, 2); short value = BitConverter.ToInt16(buffer, offset); offset += 2; return value; }");
            w.Line("private static ushort ReadUInt16(byte[] buffer, ref int offset) { Require(buffer, offset, 2); ushort value = BitConverter.ToUInt16(buffer, offset); offset += 2; return value; }");
            w.Line("private static int ReadInt32(byte[] buffer, ref int offset) { Require(buffer, offset, 4); int value = BitConverter.ToInt32(buffer, offset); offset += 4; return value; }");
            w.Line("private static uint ReadUInt32(byte[] buffer, ref int offset) { Require(buffer, offset, 4); uint value = BitConverter.ToUInt32(buffer, offset); offset += 4; return value; }");
            w.Line("private static long ReadInt64(byte[] buffer, ref int offset) { Require(buffer, offset, 8); long value = BitConverter.ToInt64(buffer, offset); offset += 8; return value; }");
            w.Line("private static ulong ReadUInt64(byte[] buffer, ref int offset) { Require(buffer, offset, 8); ulong value = BitConverter.ToUInt64(buffer, offset); offset += 8; return value; }");
            w.Line("private static float ReadFloat(byte[] buffer, ref int offset) { Require(buffer, offset, 4); float value = BitConverter.ToSingle(buffer, offset); offset += 4; return value; }");
            w.Line("private static double ReadDouble(byte[] buffer, ref int offset) { Require(buffer, offset, 8); double value = BitConverter.ToDouble(buffer, offset); offset += 8; return value; }");
            w.Line();
            w.Line("private static void WriteBytes(byte[] buffer, ref int offset, byte[] value)");
            w.BeginBlock();
            w.Line("Require(buffer, offset, value.Length);");
            w.Line("Buffer.BlockCopy(value, 0, buffer, offset, value.Length);");
            w.Line("offset += value.Length;");
            w.EndBlock();
        }
    }

    internal sealed class CodeWriter
    {
        private readonly StringBuilder _builder = new StringBuilder();
        private readonly string _indentText;
        private int _indent;

        public CodeWriter() : this("    ") { }

        public CodeWriter(string indentText)
        {
            _indentText = indentText;
        }

        public void Indent() { ++_indent; }
        public void Unindent() { if (_indent > 0) --_indent; }

        public void BeginBlock()
        {
            Line("{");
            Indent();
        }

        public void EndBlock()
        {
            Unindent();
            Line("}");
        }

        public void Line() { _builder.AppendLine(); }

        public void Line(string text)
        {
            for (int i = 0; i < _indent; ++i)
                _builder.Append(_indentText);
            _builder.AppendLine(text);
        }

        public override string ToString()
        {
            return _builder.ToString();
        }
    }

    internal static class PathUtil
    {
        public static string FindRoot(string protocolRelativePath)
        {
            DirectoryInfo dir = new DirectoryInfo(AppDomain.CurrentDomain.BaseDirectory);
            while (dir != null)
            {
                if (File.Exists(Combine(dir.FullName, protocolRelativePath)))
                    return dir.FullName;
                dir = dir.Parent;
            }
            throw new Exception("Solution root not found. Expected file: " + protocolRelativePath);
        }

        public static string Combine(string root, string relativePath)
        {
            return Path.Combine(root, relativePath.Replace('/', Path.DirectorySeparatorChar));
        }

        public static void EnsureParentDirectory(string filePath)
        {
            string dir = Path.GetDirectoryName(Path.GetFullPath(filePath));
            if (!string.IsNullOrEmpty(dir) && !Directory.Exists(dir))
                Directory.CreateDirectory(dir);
        }
    }

    internal static class TextUtil
    {
        public static string StripComment(string line)
        {
            int slash = line.IndexOf("//", StringComparison.Ordinal);
            int hash = line.IndexOf('#');
            int cut = -1;

            if (slash >= 0)
                cut = slash;
            if (hash >= 0 && (cut < 0 || hash < cut))
                cut = hash;

            return cut >= 0 ? line.Substring(0, cut) : line;
        }

        public static string TrimStatement(string text)
        {
            text = text.Trim();
            if (text.EndsWith(";", StringComparison.Ordinal))
                text = text.Substring(0, text.Length - 1).Trim();
            return text;
        }

        public static int ParseIntLiteral(string text, int lineNumber)
        {
            int value;
            if (text.StartsWith("0x", StringComparison.OrdinalIgnoreCase))
            {
                if (!int.TryParse(text.Substring(2), NumberStyles.HexNumber, CultureInfo.InvariantCulture, out value))
                    throw new Exception("Line " + lineNumber + ": invalid hex number: " + text);
                return value;
            }

            if (!int.TryParse(text, NumberStyles.Integer, CultureInfo.InvariantCulture, out value))
                throw new Exception("Line " + lineNumber + ": invalid number: " + text);
            return value;
        }

        public static int IndexOfWhiteSpace(string text, int start)
        {
            for (int i = start; i < text.Length; ++i)
            {
                if (char.IsWhiteSpace(text[i]))
                    return i;
            }
            return -1;
        }

        public static string[] SplitByComma(string text)
        {
            List<string> list = new List<string>();
            int start = 0;
            for (int i = 0; i < text.Length; ++i)
            {
                if (text[i] == ',')
                {
                    list.Add(text.Substring(start, i - start));
                    start = i + 1;
                }
            }
            list.Add(text.Substring(start));
            return list.ToArray();
        }

        public static string[] SplitByWhiteSpace(string text)
        {
            List<string> list = new List<string>();
            int i = 0;
            while (i < text.Length)
            {
                while (i < text.Length && char.IsWhiteSpace(text[i]))
                    ++i;
                if (i >= text.Length)
                    break;
                int start = i;
                while (i < text.Length && !char.IsWhiteSpace(text[i]))
                    ++i;
                list.Add(text.Substring(start, i - start));
            }
            return list.ToArray();
        }

        public static bool IsIdentifier(string text)
        {
            if (string.IsNullOrEmpty(text))
                return false;
            if (!(char.IsLetter(text[0]) || text[0] == '_'))
                return false;
            for (int i = 1; i < text.Length; ++i)
            {
                char ch = text[i];
                if (!(char.IsLetterOrDigit(ch) || ch == '_'))
                    return false;
            }
            return true;
        }

        public static string ToSnakeUpper(string text)
        {
            if (string.IsNullOrEmpty(text))
                return string.Empty;

            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < text.Length; ++i)
            {
                char ch = text[i];
                if (char.IsUpper(ch) && i > 0)
                {
                    char prev = text[i - 1];
                    if (char.IsLower(prev) || char.IsDigit(prev))
                        builder.Append('_');
                }
                builder.Append(char.ToUpperInvariant(ch));
            }
            return builder.ToString();
        }
    }
}
