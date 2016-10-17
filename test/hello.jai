#import "io"

node :: struct {
	left!: node*;
	right!: node*;
}

main :: () {
	i: int = 42;
	myVar: float = 13.4f;
	j := 13.2;
	typeCast := cast(int)myVar;
	print("Hello world\n");
}
