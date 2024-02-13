package xxl.core;

import java.util.*;

public class Product extends RangeFunction {
    
    public Product() {
        super("PRODUCT");
    }

    public Product(Range range) {
        this();
        setRange(range);
    }


    @Override
    protected Literal compute() {
        List<Cell> cells = getArg().getCells();
        int product = 1;

        for (Cell c: cells) {
            product *= c.value().asInt(); 
            /** let it throw exception if value is string */
            /** handled in view() */
        }

        return new LiteralInteger(product);
    }
}