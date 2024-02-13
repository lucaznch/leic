package xxl.core;

public abstract class RangeFunction extends Function {
    private Range _range;

    public RangeFunction(String name) {
        super(name);
    }

    public void setRange(Range range) {
        _range = range;
    }

    public Range getArg() {
        return _range;
    }

    @Override
    public String toString() {
        int fromRow = _range.getAddress(1);
        int fromColumn = _range.getAddress(2);
        int toRow = _range.getAddress(3);
        int toColumn = _range.getAddress(4);

        return "=" + getName() + "(" + fromRow + ";" + fromColumn + ":" + toRow + ";" + toColumn + ")";
    }
    
}