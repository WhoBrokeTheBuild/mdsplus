package mds.data.descriptor_r;

import java.nio.ByteBuffer;
import mds.data.DTYPE;
import mds.data.descriptor.Descriptor;
import mds.data.descriptor.Descriptor_R;

public final class With_Units extends Descriptor_R<Number> implements PARAMETER
{
	public With_Units(final ByteBuffer b)
	{
		super(b);
	}

	public With_Units(final Descriptor<?>... arguments)
	{
		super(DTYPE.WITH_UNITS, null, arguments);
	}

	public With_Units(final Descriptor<?> value, final Descriptor<?> units)
	{
		super(DTYPE.WITH_UNITS, null, value, units);
	}

	public With_Units(final Descriptor<?> value, final String units)
	{
		this(value, Descriptor.valueOf(units));
	}

	@Override
	public StringBuilder decompile(final int prec, final StringBuilder pout, final int mode)
	{
		return Descriptor_R.decompile_build(this, prec, pout, mode);
	}

	@Override
	public final Descriptor<?> getData_(final DTYPE... omits)
	{
		return this.getValue().getData(omits);
	}

	@Override
	public final With_Units getLocal_(final FLAG local)
	{
		final FLAG mylocal = new FLAG();
		final Descriptor<?> value = Descriptor.getLocal(mylocal, this.getValue());
		final Descriptor<?> units = Descriptor.getLocal(mylocal, this.getUnits());
		if (FLAG.and(local, mylocal.flag))
			return (With_Units) this.setLocal();
		return (With_Units) new With_Units(value, units).setLocal();
	}

	@Override
	public final int[] getShape()
	{ return this.getValue().getShape(); }

	@Override
	public final Descriptor<?> getUnits()
	{ return this.getDescriptor(1); }

	@Override
	public final Descriptor<?> getValue()
	{ return this.getDescriptor(0); }
}
