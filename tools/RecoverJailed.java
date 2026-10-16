// Recover original editor functions. Arguments: labels TSV, output directory.
// @category CellBlockA
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.SourceType;
import java.nio.file.*;
public class RecoverJailed extends GhidraScript {
 private ghidra.app.util.cparser.C.CParser parser;
 private void global(long address,String name,String declaration) throws Exception {
  var type=parser.parse(declaration);
  var dimensions=java.util.regex.Pattern.compile("\\[(\\d+)\\]").matcher(declaration);
  var counts=new java.util.ArrayList<Integer>();
  while(dimensions.find())counts.add(Integer.parseInt(dimensions.group(1)));
  if(!(type instanceof ghidra.program.model.data.Array))for(int i=counts.size()-1;i>=0;--i)type=new ghidra.program.model.data.ArrayDataType(type,counts.get(i),type.getLength());
  clearListing(toAddr(address),toAddr(address+type.getLength()-1));createData(toAddr(address),type);createLabel(toAddr(address),name,true);
 }

 public void run() throws Exception {
  String[] args=getScriptArgs();
  for(String line:Files.readAllLines(Path.of(args[0]))) {
   String[] f=line.split("\t"); long a=Long.parseLong(f[0],16);
   if(f[1].equals("1")) {var fn=getFunctionAt(toAddr(a)); if(fn==null){disassemble(toAddr(a));fn=createFunction(toAddr(a),f[2]);} if(fn!=null){for(var symbol:currentProgram.getSymbolTable().getSymbols(toAddr(a))) if(symbol.getSymbolType()==ghidra.program.model.symbol.SymbolType.LABEL && symbol.getName().equals(f[2]))symbol.delete();fn.setName(f[2],SourceType.USER_DEFINED);}}
   else {try {createLabel(toAddr(a),f[2],true);} catch(ghidra.util.exception.DuplicateNameException e) {}}
  }
  if(args.length>2) {
   parser=new ghidra.app.util.cparser.C.CParser(currentProgram.getDataTypeManager());
   parser.parse(new java.io.FileInputStream(args[2]));
   for(var entry:parser.getFunctions().entrySet()) {
    var symbols=currentProgram.getSymbolTable().getSymbols(entry.getKey());
    while(symbols.hasNext()){var sym=symbols.next();var f=getFunctionAt(sym.getAddress());if(f!=null)new ghidra.app.cmd.function.ApplyFunctionSignatureCmd(f.getEntryPoint(),(ghidra.program.model.data.FunctionDefinition)entry.getValue(),SourceType.USER_DEFINED).applyTo(currentProgram);}
   }
   global(0x79a68,"_map","Tile tiles[20][13];");
   global(0x79c84,"_start","Start starts[4];");
   global(0x79c80,"_buffer","BITMAP *buffer;");
   global(0x79c9c,"_game_data","DATAFILE *data;");
   global(0x7c268,"_key","unsigned char key[128];");
   global(0x59af8,"_screen","BITMAP *screen;");
   global(0x59af4,"_gfx_driver","GFX_DRIVER *driver;");
  }
  analyzeAll(currentProgram);
  var d=new DecompInterface();d.openProgram(currentProgram);Files.createDirectories(Path.of(args[1]));
  for(var f:currentProgram.getFunctionManager().getFunctions(true)) {
   long a=f.getEntryPoint().getOffset();if(a<0x1574||a>=0x2ffc)continue;
   var r=d.decompileFunction(f,60,monitor);if(!r.decompileCompleted())throw new Exception("Failed "+f.getName());
   Files.writeString(Path.of(args[1],f.getName()+".c"),"/* JAILED.EXE address: "+f.getEntryPoint()+" */\n"+r.getDecompiledFunction().getC());
  }
  d.dispose();
 }
}
