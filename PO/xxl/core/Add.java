package xxl.core;

public class Add extends BinaryFunction {

    public Add() {
        super("ADD");
    }


    public Add(Content arg1, Content arg2) {
        this();
        setArguments(arg1, arg2);
    }
    

    /** @return - value of computation */
    @Override
    protected Literal compute() {
        int number = getArg(1) + getArg(2);
            /** let it throw exception if one of args is string */

        LiteralInteger computation = new LiteralInteger(number);
        
        return computation;
    }
}