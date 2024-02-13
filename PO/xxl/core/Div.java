package xxl.core;

public class Div extends BinaryFunction {

    public Div() {
        super("DIV");
    }


    public Div(Content arg1, Content arg2) {
        this();
        setArguments(arg1, arg2);
    }


    /** @return - value of computation */
    @Override
    protected Literal compute() {
        int number = getArg(1) / getArg(2);

        LiteralInteger computation = new LiteralInteger(number);
        
        return computation;
    }
}