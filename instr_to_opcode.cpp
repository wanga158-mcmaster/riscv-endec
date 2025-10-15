#include "bits/stdc++.h"

using namespace std;

/* assumptions are listed below.
1. the instruction exists (no pseudoinstructions)
2. the instructions are separated by a newline character
3. the instruction is in lowercase
4. the register is named x0, x1, etc
5. the registers/immediate are separated by just a comma

examples:

addi x0,x0,0
add x3,x3,x3

time complexity is o(n) where n is length of instruction string,
but the constant factor could be improved (by a lot)

*/

string instr;
unordered_map<string, string> fmts; // maps instruction to format, i.e. add to r-type
unordered_map<string, string> op_types;
unordered_map<string, string> opcodes;
unordered_map<string, string> funct3s;
unordered_map<string, string> funct7s;
vector<int> commas, parens[2]; // parens[0] -> open parenthesis, parens[1] -> close parenthesis
string cur_op, cur_op_type, cur_fmt, cur_opcode, cur_funct3, cur_funct7;
string cur_rd, cur_rs1, cur_rs2;
string cur_decoded_op;
int reg1, reg2, reg3, imm;

vector<int> ans;

#define setup_fmt(_t) setup_fmts(_t, #_t)
#define setup_op_type(_t) setup_op_types(_t, #_t);

void setup_fmts(const vector<string> & ops, const string & fmt_type) {
    for (auto & op : ops) {
        fmts.insert({op, fmt_type});
    }
}

void setup_all_fmts() {
    vector<string> r_type = {
        "add",
        "sub",
        "xor",
        "or",
        "and",
        "sll",
        "srl",
        "sra",
        "slt",
        "sltu",
        "mul",
        "div",
        "rem"
    };
    vector<string> i_type = {
        "addi",
        "xori",
        "ori",
        "andi",
        "slli",
        "srli",
        "srai",
        "slti",
        "sltui",
        "lb",
        "lh",
        "lw",
        "lbu",
        "lhu",
        "jalr"
    };
    vector<string> s_type = {
        "sb",
        "sh",
        "sw"
    };
    vector<string> b_type = {
        "beq",
        "bne",
        "blt",
        "bge",
        "bltu",
        "bgeu"
    };
    vector<string> j_type = {
        "jal"
    };
    vector<string> u_type = {
        "lui",
        "auipc"
    };

    setup_fmt(r_type);
    setup_fmt(i_type);
    setup_fmt(s_type);
    setup_fmt(b_type);
    setup_fmt(j_type);
    setup_fmt(u_type);
}

void setup_op_types(const vector<string> & ops, const string & op_type) {
    for (auto & op : ops) {
        op_types.insert({op, op_type});
    }
}

void setup_all_op_types() {
    vector<string> rrr = {
        "add",
        "sub",
        "xor",
        "or",
        "and",
        "sll",
        "srl",
        "sra",
        "slt",
        "sltu",
        "mul",
        "div",
        "rem"
    };
    vector<string> rri = {
        "addi",
        "xori",
        "ori",
        "andi",
        "slli",
        "srli",
        "srai",
        "slti",
        "sltui",
        "beq",
        "bne",
        "blt",
        "bge",
        "bltu",
        "bgeu",
    };
    vector<string> rir = {
        "lb",
        "lh",
        "lw",
        "lbu",
        "lhu",
        "sb",
        "sh",
        "sw",
        "jalr"
    };
    vector<string> ri = {
        "jal",
        "lui",
        "auipc"
    };
    
    setup_op_type(rrr);
    setup_op_type(rri);
    setup_op_type(rir);
    setup_op_type(ri);

}

void setup_opcode(const vector<string> & ops, const string & opcode) {
    for (auto & op : ops) {
        opcodes.insert({op, opcode});
    }
}

void setup_all_opcodes() {
    vector<string> op0110011 = {
        "add",
        "sub",
        "xor",
        "or",
        "and",
        "sll",
        "srl",
        "sra",
        "slt",
        "sltu",
        "mul",
        "div",
        "rem"
    };
    vector<string> op0010011 = {
        "addi",
        "xori",
        "ori",
        "andi",
        "slli",
        "srli",
        "srai",
        "slti",
        "sltui",
    };
    vector<string> op0000011 = {
        "lb",
        "lh",
        "lw",
        "lbu",
        "lhu"
    };
    vector<string> op0100011 = {
        "sb",
        "sh",
        "sw"
    };
    vector<string> op1100011 = {
        "beq",
        "bne",
        "blt",
        "bge",
        "bltu",
        "bgeu"
    };
    vector<string> op1101111 = {
        "jal"
    };
    vector<string> op1100111 = {
        "jalr"
    };
    vector<string> op0110111 = {
        "lui"
    };
    vector<string> op0010111 = {
        "auipc"
    };

    setup_opcode(op0110011, "0110011");
    setup_opcode(op0010011, "0010011");
    setup_opcode(op0000011, "0000011");
    setup_opcode(op0100011, "0100011");
    setup_opcode(op1100011, "1100011");
    setup_opcode(op1101111, "1101111");
    setup_opcode(op1100111, "1100111");
    setup_opcode(op0110111, "0110111");
    setup_opcode(op0010111, "0010111");
}

void setup_funct3(const string & op, const string & funct3) {
    funct3s.insert({op, funct3});
}

void setup_all_funct3s() {
    setup_funct3("add", "000");
    setup_funct3("sub", "000");
    setup_funct3("xor", "100");
    setup_funct3("or", "110");
    setup_funct3("and", "111");
    setup_funct3("sll", "001");
    setup_funct3("srl", "101");
    setup_funct3("sra", "101");
    setup_funct3("slt", "010");
    setup_funct3("sltu", "011");
    setup_funct3("addi", "000");
    setup_funct3("xori", "100");
    setup_funct3("ori", "110");
    setup_funct3("andi", "111");
    setup_funct3("slli", "001");
    setup_funct3("srli", "101");
    setup_funct3("srai", "101");
    setup_funct3("slti", "010");
    setup_funct3("sltiu", "011");
    setup_funct3("lb", "000");
    setup_funct3("lh", "001");
    setup_funct3("lw", "010");
    setup_funct3("lbu", "100");
    setup_funct3("lhu", "101");
    setup_funct3("sb", "000");
    setup_funct3("sh", "001");
    setup_funct3("sw", "010");
    setup_funct3("beq", "000");
    setup_funct3("bne", "001");
    setup_funct3("blt", "100");
    setup_funct3("bge", "101");
    setup_funct3("bltu", "110");
    setup_funct3("bgeu", "111");
    setup_funct3("jalr", "000");
}

void setup_funct7(const string & op, const string & funct7) {
    funct7s.insert({op, funct7});
}

void setup_all_funct7s() {
    setup_funct7("add", "0000000");
    setup_funct7("sub", "0100000");
    setup_funct7("xor", "0000000");
    setup_funct7("or", "0000000");
    setup_funct7("and", "0000000");
    setup_funct7("sll", "0000000");
    setup_funct7("srl", "0000000");
    setup_funct7("sra", "0100000");
    setup_funct7("slt", "0000000");
    setup_funct7("sltu", "0000000");
}

void get_instr() {
    cur_op.clear();
    int ind;
    for (ind = 0; instr[ind] != ' '; ++ind) {
        cur_op.push_back(instr[ind]);
    } // get instruction
    // remove instruction from operation
    instr = instr.substr(ind + 1);
    cout << instr << " \n";
    // addi_x0,x0,0
    //     ^
    //     |
    //    ind
}

void get_sep() {
    commas.clear();
    parens[0].clear();
    parens[1].clear();
    for (int i = 0; i < instr.length(); ++i) {
        if (instr[i] == ',') commas.push_back(i);
        else if (instr[i] == '(') parens[0].push_back(i);
        else if (instr[i] == ')') parens[1].push_back(i);
    }
}

void get_rrr() { // instructions with reg,reg,reg
    get_sep();
    reg1 = stoi(instr.substr(1, commas[0] - 1));
    reg2 = stoi(instr.substr(commas[0] + 2, commas[1] - (commas[0] + 2)));
    reg3 = stoi(instr.substr(commas[1] + 2));
}

void get_rri() { // instructions with reg,reg,reg
    get_sep();
    reg1 = stoi(instr.substr(1, commas[0] - 1));
    reg2 = stoi(instr.substr(commas[0] + 2, commas[1] - (commas[0] + 2)));
    imm = stoi(instr.substr(commas[1] + 1));
}

void get_rir() { // instructions with reg,imm(reg)
    get_sep();
    reg1 = stoi(instr.substr(1, commas[0] - 1));
    imm = stoi(instr.substr(commas[0] + 1, parens[0][0] - (commas[0] + 1)));
    reg2 = stoi(instr.substr(parens[0][0] + 2, parens[1][0] - (parens[0][0] + 1)));
}

void get_ri() { // instructions with reg,imm
    get_sep();
    reg1 = stoi(instr.substr(1, commas[0] - 1));
    imm = stoi(instr.substr(commas[0] + 1));
}

int main()
{
    ifstream _IN("IN.txt");
    ofstream _OUT("OUT.txt");

    setup_all_fmts();
    setup_all_op_types();
    setup_all_opcodes();
    
    while (getline(_IN, instr)) {
        /* get operation */
        get_instr();
        cur_op_type = op_types[cur_op];
        cur_fmt = fmts[cur_op];
        cur_funct3 = funct3s[cur_op];
        cur_funct7 = funct7s[cur_op];
        cur_opcode = opcodes[cur_op];
        reg1 = reg2 = reg3 = imm = 0;
        if (cur_op_type == "rrr") {
            get_rrr();
        } else if (cur_op_type == "rri") {
            get_rri();
        } else if (cur_op_type == "rir") {
            get_rir();
        } else if (cur_op_type == "ri") {
            get_ri();
        }
        if (cur_fmt == "r_type") {

        } else if (cur_fmt == "i_type") {

        } else if (cur_fmt == "s_type") {

        } else if (cur_fmt == "b_type") {

        } else if (cur_fmt == "u_type") {
            
        } else if (cur_fmt == "j_type") {

        }
    }
    assert(_IN.eof());
    return 0;
}