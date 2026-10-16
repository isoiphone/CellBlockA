// @category CellBlockA
import ghidra.app.script.GhidraScript;
import ghidra.app.util.cparser.C.CParser;
import ghidra.app.cmd.function.ApplyFunctionSignatureCmd;
import ghidra.program.model.data.*;
import ghidra.program.model.symbol.SourceType;
import java.io.*;
import java.nio.file.*;

public class TypeRecovery extends GhidraScript {
    private CParser parser;
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
        global(0x80e74,"_player","Player players[2];");
        global(0x80f54,"_point","Point points[256];");
        global(0x80c6c,"_map","Tile tiles[20][13];");
        global(0x82b54,"_start","Start starts[4];");
        global(0x80f24,"_selector","Selector selector;");
        global(0x80f34,"_game_config","Config config;");
        global(0x80c18,"_maphead","FileNode maphead;");
        global(0x82b6c,"_modhead","FileNode modhead;");
        global(0x82bc8,"_game_data","DATAFILE *game_data;");
        global(0x82bc4,"_menu_data","DATAFILE *menu_data;");
        global(0x80f44,"_buffer","BITMAP *buffer;");
        global(0x80f4c,"_cur_map","char *cur_map;");
        global(0x82b64,"_music","MOD *music;");
        global(0x82bcc,"_cursong","FileNode *cursong;");
        global(0x85194,"_key","unsigned char key[128];");
        global(0x5ff1c,"_gfx_driver","GFX_DRIVER *gfx_driver;");
        global(0x5ff20,"_screen","BITMAP *screen;");
        global(0x80c10,"_game_time","double game_time;");
        global(0x5fae4,"_match_start","int match_start;");
        runScript("Recover.java",new String[] {args[1],args[2]});
    }
}
