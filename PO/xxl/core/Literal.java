package xxl.core;

public abstract class Literal extends Content {
    
    /** @return - will return a LiteralInteger or a LiteralString */
    @Override
    Literal value() {
        return this;
    }
}