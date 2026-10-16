// Export named original game functions as evidence, not hand-written substitutes.
// @category CellBlockA
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.SourceType;
import java.io.*;
import java.nio.file.*;

public class Recover extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        for (String line : Files.readAllLines(Path.of(args[0]))) {
            String[] fields = line.split("\t");
            long value = Long.parseLong(fields[0], 16);
            if (fields[1].equals("1") && value >= 0x1574 && value <= 0xbc3c) {
                var address = toAddr(value);
                disassemble(address);
                Function f = getFunctionAt(address);
                if (f == null) f = createFunction(address, fields[2]);
                if (f != null) f.setName(fields[2], SourceType.USER_DEFINED);
            }
        }
        analyzeAll(currentProgram);
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        File out = new File(args[1]);
        out.mkdirs();
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
            long address = f.getEntryPoint().getOffset();
            if ((address < 0x1574 || address > 0xbc3c) && !java.util.Set.of(
                "_rand", "_srand", "_fixtoi", "_itofix", "_ftofix", "_fcos", "_fsin", "_fatan2",
                "_fade_in", "_fade_out", "_fade_from_range").contains(f.getName())) continue;
            DecompileResults result = decomp.decompileFunction(f, 60, monitor);
            if (!result.decompileCompleted()) { println("FAILED " + f.getName()); continue; }
            Files.writeString(new File(out, f.getName() + ".c").toPath(),
                "/* Original CELL.EXE address: " + f.getEntryPoint() + " */\n" + result.getDecompiledFunction().getC());
            println("RECOVERED " + f.getName());
        }
        decomp.dispose();
    }
}
