import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class EndiannessTest {
    @Test
    public void testBigEndianValue() {
        assertEquals(0x12345678,
                Endianness.bigEndianValue(new Byte[] { (byte) 0x12, (byte) 0x34, (byte) 0x56, (byte) 0x78 }));
        assertEquals(0x00ff0000,
                Endianness.bigEndianValue(new Byte[] { (byte) 0x00, (byte) 0xff, (byte) 0x00, (byte) 0x00 }));
        assertEquals(0x8090A0B0,
                Endianness.bigEndianValue(new Byte[] { (byte) 0x80, (byte) 0x90, (byte) 0xA0, (byte) 0xB0 }));
    }

    @Test
    public void testLittleEndianValue() {
        assertEquals(0x12345678,
                Endianness.littleEndianValue(new Byte[] { (byte) 0x78, (byte) 0x56, (byte) 0x34, (byte) 0x12 }));
        assertEquals(0x00ff0000,
                Endianness.littleEndianValue(new Byte[] { (byte) 0x00, (byte) 0x00, (byte) 0xff, (byte) 0x00 }));
        assertEquals(0xB0A09080,
                Endianness.littleEndianValue(new Byte[] { (byte) 0x80, (byte) 0x90, (byte) 0xA0, (byte) 0xB0 }));
    }
}
