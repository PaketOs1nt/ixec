from argparse import ArgumentParser
from dataclasses import dataclass

OPS = [
    "NOP",
    "PUSH",
    "POP_TOP",
    "POS_ADD",
    "POS_SUB",
    "POS_ADD_IF_POP",
    "POS_SUB_IF_POP",
    "I_OP",
    "PRINT",
    "PRINTLN",
    "PRINTCH",
]

IOP = ["ADD", "SUB", "MUL", "DIV", "EQ", "NEQ"]


@dataclass
class Instr:
    op: int
    arg: int

    def to_bytes(self):
        return self.op.to_bytes(1, "little") + self.arg.to_bytes(
            4, "little", signed=True
        )

    @staticmethod
    def load(op: str, arg: int | str):
        if isinstance(arg, str):
            return Instr(OPS.index(op), IOP.index(arg))

        return Instr(OPS.index(op), arg)

    @staticmethod
    def from_asm(data: str):
        op, arg = data.split(" ", maxsplit=2)
        arg = arg.strip()
        if arg.isdigit():
            arg = int(arg)

        return Instr.load(op, arg)


@dataclass
class Code:
    stacksize: int
    code: list[Instr]

    def compile(self):
        out = []
        out += self.stacksize.to_bytes(8, "little")

        out += len(self.code).to_bytes(8, "little")
        for instr in self.code:
            out += instr.to_bytes()

        return bytes(out)


def compile(asm: str) -> Code:
    asm = asm.strip()
    instrs = []

    for line in asm.splitlines():
        instrs.append(Instr.from_asm(line.strip()))

    return Code(100, instrs)


def main():
    parser = ArgumentParser(description="iXec compiler")
    parser.add_argument("file", help="main iASM file")
    parser.add_argument("output", help="output file")

    args = parser.parse_args()

    with open(args.file, "rb") as f:
        asm = f.read().decode()

    code = compile(asm)

    with open(args.output, "wb") as f:
        f.write(code.compile())

    print("saved as", args.output)


if __name__ == "__main__":
    main()
