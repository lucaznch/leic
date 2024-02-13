package xxl.core;

public class Mul extends BinaryFunction {

    public Mul() {
        super("MUL");
    }

    
    public Mul(Content arg1, Content arg2) {
        this();
        setArguments(arg1, arg2);
    }


    /** @return - value of computation */
    @Override
    protected Literal compute() {
        int number = getArg(1) * getArg(2);

        LiteralInteger computation = new LiteralInteger(number);
        
        return computation;
    }
}