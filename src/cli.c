#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "texpackr/texpackr.h"

#define ARGSTR(i)	kArgStrings[i]
#define LARGSTR(i) strlen(kArgStrings[i])

/// width & height that will be used in producing output sheet if not specify via command line
#define DEFAULT_TARGET_WIDTH 1024
#define DEFAULT_TARGET_HEIGHT 1024

enum ArgStringIndex
{
	ARG_STR_INDEX_HELP,
	ARG_STR_INDEX_FORMAT,
	ARG_STR_INDEX_SIZE,
	ARG_STR_INDEX_PACK,
	ARG_STR_INDEX_META
};

const char* kArgStrings[] = {
	"--help",
	"--format",
	"--size",
	"--pack",
	"--meta"
};

static void	print_help_str();

void print_help_str()
{
	printf("usage: texpackr [option] command input_file ... output_file\n\n");
	printf("Commands:\n");
	printf("  %-14s show this message\n", ARGSTR(ARG_STR_INDEX_HELP));
	printf("  %-14s pack input images and compress it according to format set via %s then write as output file\n", ARGSTR(ARG_STR_INDEX_PACK), ARGSTR(ARG_STR_INDEX_FORMAT));
	printf("\n");
	printf("Options:\n");
	printf("  %-14s choose which output format to export. It can be png, or dds.\n", ARGSTR(ARG_STR_INDEX_FORMAT));
	printf("  %-14s meta output file name. If not specified, then it will use output (sheet) filename but with extension of .tpr\n", ARGSTR(ARG_STR_INDEX_META));
	printf("  %-14s specify sheet maximum width and height in format widthxheight.\n", ARGSTR(ARG_STR_INDEX_SIZE));
}

int main(int argc, char** argv)
{
	// check if not enough number of command line's argument supplied
	if (argc < 2)
	{
		fprintf(stderr, "Usage: texpackr %s\n", ARGSTR(ARG_STR_INDEX_HELP));
		return -1;
	}
	// process all command line's arguments and give priority to certain commands
	else
	{
		bool topack = false;
		int input_file_starting_index = -1;
		int target_width = -1, target_height = -1;
		char* meta_filename_ptr = NULL;

		for (int i=1; i<argc; ++i)
		{
			// give priority to help command
			if (strncmp(argv[i], ARGSTR(ARG_STR_INDEX_HELP), LARGSTR(ARG_STR_INDEX_HELP)) == 0)
			{
				print_help_str();
				// all done here, no need to continue
				return 0;
			}
			// if to pack and its flag is not set yet, as well as there're enough room ahead to specify at least one input
			// and one output
			else if (strncmp(argv[i], ARGSTR(ARG_STR_INDEX_PACK), LARGSTR(ARG_STR_INDEX_PACK)) == 0 && !topack && i+2	< argc-1)
			{
				topack = true;

				// finding the starting index of input files, working from the end
				for (int h=argc-1; h>=i; --h)
				{
					if (strncmp(argv[h], "--", 2) == 0)
					{
						if (strncmp(argv[h], ARGSTR(ARG_STR_INDEX_PACK), LARGSTR(ARG_STR_INDEX_PACK)) == 0)
							input_file_starting_index = h + 1;
						else
							input_file_starting_index = h + 2;
						printf("starting index for file input: %d\n", input_file_starting_index);
						break;
					}
				}
			}
			// get meta output filename
			else if (strncmp(argv[i], ARGSTR(ARG_STR_INDEX_META), LARGSTR(ARG_STR_INDEX_META)) == 0 && i+1 < argc)
			{
				meta_filename_ptr = argv[i+1];	
				printf("hey hey => %s\n", meta_filename_ptr);
			}
			// get sheet's width and height
			else if (strncmp(argv[i], ARGSTR(ARG_STR_INDEX_SIZE), LARGSTR(ARG_STR_INDEX_SIZE)) == 0 && target_width == -1 && target_height == -1 && i+1 < argc)
			{
				// split 'x' character between the number
				char temp[10];
				// make a copy of argument string as it will modify the string
				strncpy(temp, argv[i+1], strlen(argv[i+1]));
				char* ptr = strtok(temp, "x");
				const char* width = NULL;
				const char* height = NULL;

				while (ptr != NULL)
				{
					if (width == NULL)
					{
						width = ptr;
						printf("width found\n");
					}
					else
					{
						height = ptr;
						printf("height found\n");
						// for safety to make sure we break out if user weirdly enter 1024x1024x1024...
						break;
					}
					// proceed next
					ptr = strtok(NULL, "x");
				}

				// check for format invalid
				if (width == NULL || height == NULL)
				{
					fprintf(stderr, "Size input has invalid format. Make sure it's in the form numberxnumber. x is in between width and height.\n");
					return -1;
				}
				// convert string to integer
				target_width = (int)strtol(width, NULL, 10);
				target_height = (int)strtol(height, NULL, 10);

				printf("width = %d, height = %d\n", target_width, target_height);
			}
		}

		// set width and height to default ones if not specified
		if (target_width == -1)
			target_width = DEFAULT_TARGET_WIDTH;
		if (target_height == -1)
			target_height = DEFAULT_TARGET_HEIGHT;

		bool meta_dyna_filename = false;

		// check if we need to use default meta filename if not specified by user
		if (meta_filename_ptr == NULL)
		{
			// we need to free it later
			meta_dyna_filename = true;

			// temporary output name, with exact length of output's string filename
			char temp[strlen(argv[argc-1]) + 1];
			// make a copy of argument string as it will modify the string
			strncpy(temp, argv[argc-1], strlen(argv[argc-1]));
			meta_filename_ptr = strtok(temp, ".");
			printf("------> %s\n", meta_filename_ptr);

			// if there's no . then just use what ever output file is
			// but append with .tpr extension
			if (meta_filename_ptr == NULL)
			{
				const int size = strlen(argv[argc-1]) + 4 + 1;
				meta_filename_ptr = malloc(size);
				snprintf(meta_filename_ptr, size, "%s.tpr", argv[argc-1]);
			}
			// append with .tpr extension
			else
			{
				const int size = strlen(meta_filename_ptr) + 4 + 1;
				char* new_str = malloc(size);
				snprintf(new_str, size, "%s.tpr", meta_filename_ptr);
				// swap pointer
				meta_filename_ptr = new_str;
			}

			printf("=>  meta filename = %s\n", meta_filename_ptr);
		}

		// if all things went well
		if (topack && input_file_starting_index != -1)
		{
			// form an array for file input
			const int inputfiles_size = argc-1 - input_file_starting_index;
			printf("input file count: %d\n", inputfiles_size);
			const char* inputfiles[inputfiles_size];
			for (int i=0; i<inputfiles_size; ++i)
			{
				printf("input file[%d] = %s\n", i, argv[i+input_file_starting_index]);
				inputfiles[i] = argv[i+input_file_starting_index];
			}

			printf("output file = %s\n", argv[argc-1]);

			// create a new sheet
			texpackr_sheet* sheet = texpackr_sheet_new(target_width, target_height);
			// batch input images
			bool result = texpackr_sheet_batch_imgs(sheet, (const char**)&inputfiles, inputfiles_size);
			if (!result)
			{
				fprintf(stderr, "Faild in batch packing input images\n");

				// free meta filename if need
				if (meta_dyna_filename)
				{
					free(meta_filename_ptr);
					meta_filename_ptr = NULL;
				}

				// free sheet
				texpackr_sheet_free(sheet);
				sheet = NULL;

				return -1;
			}

			// export
			result = texpackr_sheet_export(sheet, argv[argc-1], meta_filename_ptr);
			if (!result)
			{
				fprintf(stderr, "Faild in exporting into sheet and meta file\n");

				// free meta filename if need
				if (meta_dyna_filename)
				{
					free(meta_filename_ptr);
					meta_filename_ptr = NULL;
				}

				// free sheet
				texpackr_sheet_free(sheet);
				sheet = NULL;

				return -1;
			}

			texpackr_sheet_free(sheet);
			sheet = NULL;

			// free meta filename if need
			if (meta_dyna_filename)
			{
				free(meta_filename_ptr);
				meta_filename_ptr = NULL;
			}

			printf("Done\n");
		}
		else
		{
			// free meta filename if need
			if (meta_dyna_filename)
			{
				free(meta_filename_ptr);
				meta_filename_ptr = NULL;
			}

			fprintf(stderr, "Please make sure to specify at least one input file, and output file at the end.\n");
			fprintf(stderr, "See usage via 'texpackr --help'\n");
			return -1;
		}
	}

	return 0;
}
