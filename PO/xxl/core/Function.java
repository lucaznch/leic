package xxl.core;

public abstract class Function extends Content {
    private String _name;



    public Function(String name) {
        _name = name;
    }

    public Function() {
    }


    /** @return - computes the function and returns its value, a literal */
    protected abstract Literal compute();


    /** @return - name of function */
    public String getName() {
        return _name;
    }


    /** @return - value as a string */
    @Override
    public String asString() {
        return value().asString();
    }


    /** @return - value as an int */
    @Override
    public int asInt() {
        return value().asInt();
    }


    /** @return - value of function */
    @Override
    public Literal value() {
        return compute();
    }
}