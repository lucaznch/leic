package xxl.core;

import java.util.*;

public class Coalesce extends RangeFunction {

    public Coalesce(Range range) {
        super("COALESCE");
        setRange(range);
    }


    @Override
    protected Literal compute() {
        List<Cell> cells = getArg().getCells();
        String coalesce;
        
        for (Cell c: cells) {
            try {
                coalesce = c.value().asString();
                return new LiteralString(coalesce);
            }
            catch (NumberFormatException | NullPointerException e) {
                /** do nothing */
            }
        }

        return new LiteralString("");
    }
}