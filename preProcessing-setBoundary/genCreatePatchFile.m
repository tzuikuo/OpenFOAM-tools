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
while ~feof(fid)  % ѭ��ֱ���ļ�����
    line = fgetl(fid);  % ��ȡ���У��ַ�����
    ii=ii+1;
    if isempty(line) || all(isspace(line)) || (ii<17)
        continue;  % ��������
    end
    % �� textscan ������ǰ�У�����
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
%% write cretaPatch
%
outputfile1=sprintf('batch_createPatch',caseName);
fid = fopen(outputfile1, 'w');
count=0;
for ii=1:length(bd)
    indx = regexp(bd(ii), 'gridwall-(\d{1,2})', 'tokens');
    if(isempty(indx))
        continue;
    else
        fprintf(fid,'{\n');
        fprintf(fid,sprintf('\tname interface-%s;\n',indx{1,1}));
        fprintf(fid,'\tpatchInfo\n');
        fprintf(fid,'\t{\n');
        fprintf(fid,'\t\ttype empty;\n');
        fprintf(fid,'\t}\n');
        fprintf(fid,'\tconstructFrom patches;\n');
        fprintf(fid,sprintf('\tpatches (%s);\n',bd(ii)));
        fprintf(fid,'}\n');
        count=count+1;
    end
end
count
