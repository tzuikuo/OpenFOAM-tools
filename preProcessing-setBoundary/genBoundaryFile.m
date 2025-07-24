clear all;
clc;
%% read boundary
caseName='case5-1-3-1-87w';
inputfile=sprintf('sloshingRocketZKYH/%s/constant/polyMesh/boundary',caseName);
%
fid = fopen(inputfile, 'r');
clast='';
bd=[];
ii=0;
cc=0;
while ~feof(fid)  % 循环直到文件结束
    line = fgetl(fid);  % 读取整行（字符串）
    ii=ii+1;
    if isempty(line) || all(isspace(line)) || (ii<17)
        continue;  % 跳过空行
    end
    exp0= regexp(line, 'interface-(\d{1,2})', 'tokens');
    if(~isempty(exp0))
        cc=cc+1;
    end
    % 用 textscan 解析当前行（更灵活）
    snew = strip(string(line),'left');
    cnew=char(snew);
    cnew=cnew(1);
    if (strcmp(clast,'(')||strcmp(clast,'}'))
        bd=[bd;strip(snew)];
    end
    clast=cnew;
    %disp(clast)
end
fclose(fid);
bd=bd(1:end-1);
%% write boundary
%
outputfile1=sprintf('bd_alpha.water',caseName);
fid = fopen(outputfile1, 'w');
for ii=1:length(bd)
    exp0= regexp(bd(ii), 'interface-(\d{1,2})', 'tokens');
    if(~isempty(exp0))
        continue;
    end
    if(strcmp(bd(ii),"gridWall_large")||strcmp(bd(ii),"gridWall_small"))
        fprintf(fid,'%s\n',bd(ii));
        fprintf(fid,'{\n');
        fprintf(fid,'\ttype           cyclic;\n');
        fprintf(fid,'}\n');
    else
        fprintf(fid,'%s\n',bd(ii));
        fprintf(fid,'{\n');
        fprintf(fid,'\ttype           constantAlphaContactAngle;\n');
        fprintf(fid,'\ttheta0         30;\n');
        fprintf(fid,'\tlimit          gradient;\n');
        fprintf(fid,'\tvalue          uniform 0;\n');
        fprintf(fid,'}\n');
    end
end
%
outputfile1=sprintf('bd_p_rgh',caseName);
fid = fopen(outputfile1, 'w');
for ii=1:length(bd)
    exp0= regexp(bd(ii), 'interface-(\d{1,2})', 'tokens');
    if(~isempty(exp0))
        continue;
    end
    if(strcmp(bd(ii),"gridWall_large")||strcmp(bd(ii),"gridWall_small"))
        fprintf(fid,'%s\n',bd(ii));
        fprintf(fid,'{\n');
        fprintf(fid,'\ttype           cyclic;\n');
        fprintf(fid,'}\n');
    else
        fprintf(fid,'%s\n',bd(ii));
        fprintf(fid,'{\n');
        fprintf(fid,'\ttype            fixedFluxPressure;\n');
        fprintf(fid,'}\n');
    end
end
% 
outputfile1=sprintf('bd_U',caseName);
fid = fopen(outputfile1, 'w');
for ii=1:length(bd)
    exp0= regexp(bd(ii), 'interface-(\d{1,2})', 'tokens');
    if(~isempty(exp0))
        continue;
    end
    if(strcmp(bd(ii),"gridWall_large")||strcmp(bd(ii),"gridWall_small"))
        fprintf(fid,'%s\n',bd(ii));
        fprintf(fid,'{\n');
        fprintf(fid,'\ttype           cyclic;\n');
        fprintf(fid,'}\n');
    else
        fprintf(fid,'%s\n',bd(ii));
        fprintf(fid,'{\n');
        fprintf(fid,'\ttype            noSlip;\n');
        fprintf(fid,'}\n');
    end
end
