package MDSplus;

/**
 * Class description of DTYPE_FLOAT
 *
 * Constructor: Float32(float)
 *
 * @author manduchi
 * @version 1.0
 * @updated 30-mar-2009 13.44.36
 */
public class Float32 extends Scalar
{
	float datum;

	public Float32(float datum)
	{
		this(datum, null, null, null, null);
	}

	public Float32(float datum, Data help, Data units, Data error, Data validation)
	{
		super(help, units, error, validation);
		clazz = CLASS_S;
		dtype = DTYPE_FLOAT;
		this.datum = datum;
	}

	@Override
	public boolean equals(Object data)
	{
		if (!(data instanceof Float32))
			return false;
		return ((Float32) data).datum == datum;
	}

	public static Data getData(float datum, Data help, Data units, Data error, Data validation)
	{
		return new Float32(datum, help, units, error, validation);
	}

	@Override
	public byte getByte() throws MdsException
	{ return (byte) datum; }

	/**
	 * Convert this data into a short. Implemented at this class level by returning
	 * TDI data(WORD(this)).If data() fails or the returned class is not scalar,
	 * generated an exception.
	 */
	@Override
	public short getShort() throws MdsException
	{ return (short) datum; }

	/**
	 * Convert this data into a int. Implemented at this class level by returning
	 * TDI data(LONG(this)).If data() fails or the returned class is not scalar,
	 * generated an exception.
	 */
	@Override
	public int getInt() throws MdsException
	{ return (int) datum; }

	/**
	 * Convert this data into a long. Implemented at this class level by returning
	 * TDI data(QUADWORD(this)).If data() fails or the returned class is not scalar,
	 * generated an exception.
	 */
	@Override
	public long getLong() throws MdsException
	{ return (long) datum; }

	/**
	 * Convert this data into a float. Implemented at this class level by returning
	 * TDI data(F_FLOAT(this)).If data() fails or the returned class is not scalar,
	 * generated an exception.
	 */
	@Override
	public float getFloat() throws MdsException
	{ return datum; }

	/**
	 * Convert this data into a double. Implemented at this class level by returning
	 * TDI data(FT_FLOAT(this)). If data() fails or the returned class is not
	 * scalar, generated an exception.
	 */
	@Override
	public double getDouble() throws MdsException
	{ return datum; }

	/**
	 * Convert this data into a byte array. Implemented at this class level by
	 * returning TDI data(BYTE(this)). If data() fails or the returned class is not
	 * array, generates an exception. In Java and C++ will return a 1 dimensional
	 * array using row-first ordering if a multidimensional array.
	 */
	@Override
	public byte[] getByteArray() throws MdsException
	{
		return new byte[]
		{ (byte) datum };
	}

	/**
	 * Convert this data into a short array. Implemented at this class level by
	 * returning TDI data(WORD(this)). If data() fails or the returned class is not
	 * array, generates an exception. In Java and C++ will return a 1 dimensional
	 * array using row-first ordering if a multidimensional array.
	 */
	@Override
	public short[] getShortArray() throws MdsException
	{
		return new short[]
		{ (short) datum };
	}

	/**
	 * Convert this data into a int array. Implemented at this class level by
	 * returning TDI data (LONG(this)). If data() fails or the returned class is not
	 * array, generates an exception. In Java and C++ will return a 1 dimensional
	 * array using row-first ordering if a multidimensional array.
	 */
	@Override
	public int[] getIntArray() throws MdsException
	{
		return new int[]
		{ (int) datum };
	}

	/**
	 * Convert this data into a long array. Implemented at this class level by
	 * returning TDI data(QUADWORD(this)). If data() fails or the returned class is
	 * not array, generates an exception. In Java and C++ will return a 1
	 * dimensional array using row-first ordering if a multidimensional array.
	 */
	@Override
	public long[] getLongArray() throws MdsException
	{
		return new long[]
		{ (long) datum };
	}

	/**
	 * Convert this data into a float array. Implemented at this class level by
	 * returning TDI data(QUADWORD(this)). If data() fails or the returned class is
	 * not array, generates an exception. In Java and C++ will return a 1
	 * dimensional array using row-first ordering if a multidimensional array.
	 */
	@Override
	public float[] getFloatArray() throws MdsException
	{
		return new float[]
		{ datum };
	}

	/**
	 * Convert this data into a long array. Implemented at this class level by
	 * returning TDI data(QUADWORD(this)). If data() fails or the returned class is
	 * not array, generates an exception. In Java and C++ will return a 1
	 * dimensional array using row-first ordering if a multidimensional array.
	 */
	@Override
	public double[] getDoubleArray() throws MdsException
	{
		return new double[]
		{ datum };
	}

	@Override
	public int getSizeInBytes()
	{ return 4; }
}
