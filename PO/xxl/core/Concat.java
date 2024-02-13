package xxl.core;

import java.util.*;

public class Concat extends RangeFunction {
    public Concat(Range range) {
        super("CONCAT");
        setRange(range);
    }

    @Override
    protected Literal compute() {
        List<Cell> cells = getArg().getCells();
        String concat = "";

        for (Cell c: cells) {
            try {
                concat += c.value().asString();
            }
            catch (NumberFormatException | NullPointerException  e) {
                /** do nothing */
            }
        }

        return new LiteralString(concat);
    }
}