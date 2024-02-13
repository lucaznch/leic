package xxl.core;

import java.util.*;

public class Average extends RangeFunction { 
    /** this function only works with integers */
    
    /** doesnt make sense to have this */
    public Average() {
        super("AVERAGE");
    }

    public Average(Range range) {
        this();
        setRange(range);
    }


    @Override
    protected Literal compute() {
        List<Cell> cells = getArg().getCells();
        int sum = 0;

        for (Cell c: cells) {
            sum += c.value().asInt();
            /** let it throw exception if value is string */
            /** handled in view() */
        }

        return new LiteralInteger(sum / cells.size());
    }

}