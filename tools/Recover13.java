// @category CellBlockA
import ghidra.app.script.GhidraScript;
import ghidra.app.util.cparser.C.CParser;
import ghidra.app.cmd.function.ApplyFunctionSignatureCmd;
import ghidra.program.model.data.*;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolType;
import java.io.*;
import java.nio.file.*;

public class Recover13 extends GhidraScript {
    private CParser parser;
    private void nameFunction(long offset, String name) throws Exception {
        var address=toAddr(offset);
        var f=getFunctionAt(address);
        if(f==null){disassemble(address);f=createFunction(address,name);}
        // A previous recovery pass can leave a label with the desired name.
        // Remove that alias before renaming the function so reruns are safe.
        for(var symbol:currentProgram.getSymbolTable().getSymbols(address))
            if(symbol.getSymbolType()==SymbolType.LABEL && symbol.getName().equals(name))
                symbol.delete();
        if(f!=null)f.setName(name,SourceType.USER_DEFINED);
    }
    private void global(long address, String name, String declaration) throws Exception {
        DataType type = parser.parse(declaration);
        var dimensions = java.util.regex.Pattern.compile("\\[(\\d+)\\]").matcher(declaration);
        var counts = new java.util.ArrayList<Integer>();
        while (dimensions.find()) counts.add(Integer.parseInt(dimensions.group(1)));
        if (!(type instanceof Array)) {
            for (int i = counts.size()-1; i >= 0; --i)
                type = new ArrayDataType(type,counts.get(i),type.getLength());
        }
        println("TYPE " + name + " " + type.getDisplayName() + " " + type.getLength());
        clearListing(toAddr(address), toAddr(address + type.getLength() - 1));
        createData(toAddr(address), type);
        createLabel(toAddr(address), name, true);
    }
    public void run() throws Exception {
        String[] args = getScriptArgs();
        for (String line:Files.readAllLines(Path.of(args[1]))) {
            String[] fields=line.split("\\t");
            var address=toAddr(Long.parseLong(fields[0],16));
            if(fields[1].equals("1")) {
                nameFunction(address.getOffset(),fields[2]);
            } else createLabel(address,fields[2],true);
        }

        // Manually verified library targets: instruction matching alone confused
        // similar short CRT/keyboard functions; call sites and bodies disambiguate.
        nameFunction(0x51cfc,"_printf");
        nameFunction(0x51d44,"_rawclock");
        nameFunction(0x185b0,"_clear_keybuf");
        // Byte-identical to 1.0's _draw_sprite_h_flip (0x13474): calls
        // BITMAP.vtable + 0x30. Vertical flip calls +0x2c, not +0x30.
        nameFunction(0x13c48,"_draw_sprite_h_flip");
        parser = new CParser(currentProgram.getDataTypeManager());
        parser.parse(new FileInputStream(args[0]));
        for (var entry : parser.getFunctions().entrySet()) {
            var symbols = currentProgram.getSymbolTable().getSymbols(entry.getKey());
            while (symbols.hasNext()) {
                var symbol = symbols.next();
                var f = getFunctionAt(symbol.getAddress());
                if (f == null) { disassemble(symbol.getAddress()); f = createFunction(symbol.getAddress(),entry.getKey()); }
                if (f != null) {
                    var cmd = new ApplyFunctionSignatureCmd(f.getEntryPoint(), (FunctionDefinition)entry.getValue(), SourceType.USER_DEFINED);
                    cmd.applyTo(currentProgram);
                }
            }
        }
        global(0x81674,"_player","Player players[2];");
        global(0x8174c,"_point","Point points[256];");
        global(0x8146c,"_map","Tile tiles[20][13];");
        global(0x8334c,"_start","Start starts[4];");
        global(0x81724,"_selector","Selector selector;");
        global(0x81734,"_game_config","Config config;");
        global(0x81418,"_maphead","FileNode maphead;");
        global(0x83364,"_modhead","ModNode modhead;");
        global(0x833c0,"_game_data","DATAFILE *game_data;");
        global(0x833bc,"_menu_data","DATAFILE *menu_data;");
        global(0x81744,"_buffer","BITMAP *buffer;");
        global(0x81748,"_cur_map","char *cur_map;");
        global(0x8335c,"_music","MOD *music;");
        global(0x833c4,"_cursong","ModNode *cursong;");
        global(0x85990,"_key","unsigned char key[128];");
        global(0x6071c,"_gfx_driver","GFX_DRIVER *gfx_driver;");
        global(0x60720,"_screen","BITMAP *screen;");
        global(0x81410,"_game_time","double game_time;");
        global(0x602e4,"_match_start","int match_start;");
        global(0x833c8,"native_arena_background","BITMAP *native_arena_background;");
        global(0x1e88,"native_initial_point","int native_initial_point[7];");
        analyzeAll(currentProgram);
        var decomp=new ghidra.app.decompiler.DecompInterface();
        decomp.openProgram(currentProgram);
        Files.createDirectories(Path.of(args[2]));
        for(var f:currentProgram.getFunctionManager().getFunctions(true)) {
            long a=f.getEntryPoint().getOffset();
            if(a<0x1574 || a>0xc500)continue;
            var result=decomp.decompileFunction(f,60,monitor);
            if(!result.decompileCompleted()){println("FAILED "+f.getName());continue;}
            Files.writeString(Path.of(args[2],f.getName()+".c"),"/* Original 1.3b CELL.EXE address: "+f.getEntryPoint()+" */\n"+result.getDecompiledFunction().getC());
        }
        decomp.dispose();
    }
}
