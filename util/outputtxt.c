#include "blocks/blocktypes.h"
#include "parser.h"
#include "swfoutput.h"

extern const char *blockName (SWFBlocktype header);
#ifdef NODECOMPILE
extern const char *actionName (SWFActiontype header);
#endif

/*
 * This file contains output functions that can display the different SWF block
 * types in a human readable format.
 */

#define OUT_BEGIN(block) \
	struct block *sblock = (struct block *)pblock;

static struct SWFOutput outputs[] = {
  {SWF_CHARACTERSET, outputSWF_CHARACTERSET},
  {SWF_DEFINEBITS, outputSWF_DEFINEBITS},
  {SWF_DEFINEBITSJPEG2, outputSWF_DEFINEBITSJPEG2},
  {SWF_DEFINEBITSJPEG3, outputSWF_DEFINEBITSJPEG3},
  {SWF_DEFINEBITSPTR, outputSWF_DEFINEBITSPTR},
  {SWF_DEFINEBUTTON, outputSWF_DEFINEBUTTON},
  {SWF_DEFINEBUTTON2, outputSWF_DEFINEBUTTON2},
  {SWF_DEFINEBUTTONCXFORM, outputSWF_DEFINEBUTTONCXFORM},
  {SWF_DEFINEBUTTONSOUND, outputSWF_DEFINEBUTTONSOUND},
  {SWF_DEFINECOMMANDOBJ, outputSWF_DEFINECOMMANDOBJ},
  {SWF_DEFINEEDITTEXT, outputSWF_DEFINEEDITTEXT},
  {SWF_DEFINEFONT, outputSWF_DEFINEFONT},
  {SWF_DEFINEFONT2, outputSWF_DEFINEFONT2},
  {SWF_DEFINEFONTINFO, outputSWF_DEFINEFONTINFO},
  {SWF_DEFINELOSSLESS, outputSWF_DEFINELOSSLESS},
  {SWF_DEFINELOSSLESS2, outputSWF_DEFINELOSSLESS2},
  {SWF_DEFINEMORPHSHAPE, outputSWF_DEFINEMORPHSHAPE},
  {SWF_DEFINESHAPE, outputSWF_DEFINESHAPE},
  {SWF_DEFINESHAPE2, outputSWF_DEFINESHAPE2},
  {SWF_DEFINESHAPE3, outputSWF_DEFINESHAPE3},
  {SWF_DEFINESOUND, outputSWF_DEFINESOUND},
  {SWF_DEFINESPRITE, outputSWF_DEFINESPRITE},
  {SWF_DEFINETEXT, outputSWF_DEFINETEXT},
  {SWF_DEFINETEXT2, outputSWF_DEFINETEXT2},
  {SWF_DEFINETEXTFORMAT, outputSWF_DEFINETEXTFORMAT},
  {SWF_DEFINEVIDEO, outputSWF_DEFINEVIDEO},
  {SWF_DEFINEVIDEOSTREAM, outputSWF_DEFINEVIDEOSTREAM},
  {SWF_DOACTION, outputSWF_DOACTION},
  {SWF_ENABLEDEBUGGER, outputSWF_ENABLEDEBUGGER},
  {SWF_END, outputSWF_END},
  {SWF_EXPORTASSETS, outputSWF_EXPORTASSETS},
  {SWF_FONTREF, outputSWF_FONTREF},
  {SWF_FRAMELABEL, outputSWF_FRAMELABEL},
  {SWF_FRAMETAG, outputSWF_FRAMETAG},
  {SWF_FREEALL, outputSWF_FREEALL},
  {SWF_FREECHARACTER, outputSWF_FREECHARACTER},
  {SWF_GENCOMMAND, outputSWF_GENCOMMAND},
  {SWF_IMPORTASSETS, outputSWF_IMPORTASSETS},
  {SWF_JPEGTABLES, outputSWF_JPEGTABLES},
  {SWF_NAMECHARACTER, outputSWF_NAMECHARACTER},
  {SWF_PATHSAREPOSTSCRIPT, outputSWF_PATHSAREPOSTSCRIPT},
  {SWF_PLACEOBJECT, outputSWF_PLACEOBJECT},
  {SWF_PLACEOBJECT2, outputSWF_PLACEOBJECT2},
  {SWF_PREBUILT, outputSWF_PREBUILT},
  {SWF_PREBUILTCLIP, outputSWF_PREBUILTCLIP},
  {SWF_PROTECT, outputSWF_PROTECT},
  {SWF_REMOVEOBJECT, outputSWF_REMOVEOBJECT},
  {SWF_REMOVEOBJECT2, outputSWF_REMOVEOBJECT2},
  {SWF_SERIALNUMBER, outputSWF_SERIALNUMBER},
  {SWF_SETBACKGROUNDCOLOR, outputSWF_SETBACKGROUNDCOLOR},
  {SWF_SHOWFRAME, outputSWF_SHOWFRAME},
  {SWF_SOUNDSTREAMBLOCK, outputSWF_SOUNDSTREAMBLOCK},
  {SWF_SOUNDSTREAMHEAD, outputSWF_SOUNDSTREAMHEAD},
  {SWF_SOUNDSTREAMHEAD2, outputSWF_SOUNDSTREAMHEAD2},
  {SWF_STARTSOUND, outputSWF_STARTSOUND},
  {SWF_SYNCFRAME, outputSWF_SYNCFRAME},
  {SWF_VIDEOFRAME, outputSWF_VIDEOFRAME},
};

static int numOutputs = sizeof (outputs) / sizeof (struct SWFOutput);

/* Output basic Flash Types */

void
outputSWF_RGBA (SWF_RGBA * rgb, char *pname)
{
  printf (" RGBA: ");
  printf (" Red:   %3d ", rgb->red);
  printf (" Green: %3d ", rgb->green);
  printf (" Blue:  %3d ", rgb->blue);
  printf (" Alpha  %3d\n", rgb->alpha);
}

void
outputSWF_RECT (SWF_RECT * rect)
{
  printf (" RECT: ");
  printf (" Nbits: %2d ", rect->Nbits);
  printf (" Xmin: %6ld ", rect->Xmin);
  printf (" Xmax: %6ld ", rect->Xmax);
  printf (" Ymin: %6ld ", rect->Ymin);
  printf (" Ymax: %6ld\n", rect->Ymax);
}

void
outputSWF_MATRIX (SWF_MATRIX * matrix, char *name)
{
  printf (" Matrix: ");
  if (matrix->HasScale)
    {
      printf ("ScaleX %f ", matrix->ScaleX);
      printf ("ScaleY %f\n", matrix->ScaleY);
    }
  if (matrix->HasRotate)
    {
      printf ("RotateSkew0 %f ", matrix->RotateSkew0);
      printf ("RotateSkew1 %f\n", matrix->RotateSkew1);
    }
  printf ("   TranslateX %6ld ", matrix->TranslateX);
  printf ("TranslateY %6ld\n", matrix->TranslateY);
}

void
outputSWF_GRADIENTRECORD (SWF_GRADIENTRECORD * gradientrec, char *gname)
{
  printf (" Ratio: %d\n", gradientrec->Ratio);
  outputSWF_RGBA (&gradientrec->Color, "");
}

void
outputSWF_GRADIENT (SWF_GRADIENT * gradient, char *name)
{
  int i;
  printf (" Gradient: ");
  printf (" NumGradients: %d\n", gradient->NumGradients);
  for (i = 0; i < gradient->NumGradients; i++)
    outputSWF_GRADIENTRECORD (&(gradient->GradientRecords[i]),"");
}


void
outputSWF_FILLSTYLE (SWF_FILLSTYLE * fillstyle, char *name, int i)
{
  printf (" FillStyle: ");
  printf (" FillStyleType: %x\n", fillstyle->FillStyleType);
  switch (fillstyle->FillStyleType)
    {
    case 0x00:			/* Solid Fill */
      outputSWF_RGBA (&fillstyle->Color, "");
      break;
    case 0x10:			/* Linear Gradient Fill */
    case 0x12:			/* Radial Gradient Fill */
      outputSWF_MATRIX (&fillstyle->GradientMatrix,"");
      outputSWF_GRADIENT (&fillstyle->Gradient,"");
      break;
    case 0x40:			/* Repeating Bitmap Fill */
    case 0x41:			/* Clipped Bitmap Fill */
    case 0x42:			/* Non-smoothed Repeating Bitmap Fill */
    case 0x43:			/* Non-smoothed Clipped Bitmap Fill */
      printf (" BitmapID: %d\n", fillstyle->BitmapId);
      outputSWF_MATRIX (&fillstyle->BitmapMatrix,"");
      break;
    }
}

void
outputSWF_FILLSTYLEARRAY (SWF_FILLSTYLEARRAY * fillstylearray, char *name)
{
  int count, i;
  printf (" FillStyleArray: ");
  printf (" FillStyleCount: %6d ", fillstylearray->FillStyleCount);
  printf (" FillStyleCountExtended: %6d\n",
	  fillstylearray->FillStyleCountExtended);
  count =
    (fillstylearray->FillStyleCount !=
     0xff) ? fillstylearray->FillStyleCount : fillstylearray->
    FillStyleCountExtended;
  for (i = 0; i < count; i++)
    {
      outputSWF_FILLSTYLE (&(fillstylearray->FillStyles[i]),"",0);
    }
}

void
outputSWF_LINESTYLE (SWF_LINESTYLE * fillstyle, char *name, int i)
{
  printf (" LineStyle: ");
  printf (" Width: %d\n", fillstyle->Width);
  outputSWF_RGBA (&fillstyle->Color, "");
}

void
outputSWF_LINESTYLEARRAY (SWF_LINESTYLEARRAY * linestylearray, char *name)
{
  int count, i;
  printf (" LineStyleArray: ");
  printf (" LineStyleCount: %6d ", linestylearray->LineStyleCount);
  printf (" LineStyleCountExtended: %6d\n",
	  linestylearray->LineStyleCountExtended);
  count =
    (linestylearray->LineStyleCount !=
     0xff) ? linestylearray->LineStyleCount : linestylearray->
    LineStyleCountExtended;
  for (i = 0; i < count; i++)
    {
      outputSWF_LINESTYLE (&(linestylearray->LineStyles[i]),"",0);
    }
}

void
outputSWF_SHAPERECORD (SWF_SHAPERECORD * shaperec, char *parentname)
{
  printf (" ShapeRecord:\n");
  if (shaperec->EndShape.TypeFlag)
    {
      /* An Edge Record */
      if (shaperec->StraightEdge.StraightEdge == 1)
	{
	  /* A Straight Edge Record */
	  printf ("  Straight EdgeRecord: %d\n",
		  shaperec->StraightEdge.NumBits);
	}
      else
	{
	  /* A Curved Edge Record */
	  printf ("  Curved EdgeRecord: %d", shaperec->CurvedEdge.NumBits);
	  printf (" Control(%ld,%ld)", shaperec->CurvedEdge.ControlDeltaX,
		  shaperec->CurvedEdge.ControlDeltaY);
	  printf (" Anchor(%ld,%ld)\n", shaperec->CurvedEdge.AnchorDeltaX,
		  shaperec->CurvedEdge.AnchorDeltaY);
	}
    }
  else
    {
      /* A Non-Edge Record */
      if (shaperec->EndShape.EndOfShape == 0)
	{
	  printf ("  ENDSHAPE\n");
	  return;
	}
      printf ("  StyleChangeRecord:\n");
      printf ("   StateNewStyles: %d", shaperec->StyleChange.StateNewStyles);
      printf (" StateLineStyle: %d ", shaperec->StyleChange.StateLineStyle);
      printf (" StateFillStyle1: %d\n",
	      shaperec->StyleChange.StateFillStyle1);
      printf ("   StateFillStyle0: %d",
	      shaperec->StyleChange.StateFillStyle0);
      printf (" StateMoveTo: %d\n", shaperec->StyleChange.StateMoveTo);

      if (shaperec->StyleChange.StateLineStyle) {
	  printf (" LineStyle: %ld\n", shaperec->StyleChange.LineStyle);
      }
      if (shaperec->StyleChange.StateFillStyle1) {
	  printf (" FillStyle1: %ld\n", shaperec->StyleChange.FillStyle1);
      }
      if (shaperec->StyleChange.StateFillStyle0) {
	  printf (" FillStyle0: %ld\n", shaperec->StyleChange.FillStyle0);
      }
      if (shaperec->StyleChange.StateMoveTo)
	{
	  printf (" MoveBits: %d ", shaperec->StyleChange.MoveBits);
	  printf (" MoveDeltaX: %ld ", shaperec->StyleChange.MoveDeltaX);
	  printf (" MoveDeltaY: %ld\n", shaperec->StyleChange.MoveDeltaY);
	}
    }
}

void
outputSWF_SHAPE (SWF_SHAPE * shape, char *name)
{
  int i;
  printf (" NumFillBits: %d\n", shape->NumFillBits);
  printf (" NumLineBits: %d\n", shape->NumLineBits);
  for (i = 0; i < shape->NumShapeRecords; i++)
    {
      outputSWF_SHAPERECORD (&(shape->ShapeRecords[i]), name);
    }
}

void
outputSWF_SHAPEWITHSTYLE (SWF_SHAPEWITHSTYLE * shape, int level, char *name)
{
  int i;

  outputSWF_FILLSTYLEARRAY (&(shape->FillStyles),"");
  outputSWF_LINESTYLEARRAY (&(shape->LineStyles),"");
  printf (" NumFillBits: %d\n", shape->NumFillBits);
  printf (" NumLineBits: %d\n", shape->NumLineBits);
  for (i = 0; i < shape->NumShapeRecords; i++)
    {
      outputSWF_SHAPERECORD (&(shape->ShapeRecords[i]),name);
    }
}


/* Output Flash Blocks */

void
outputSWF_CHARACTERSET (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_CHARACTERSET);

}

void
outputSWF_DEFINEBITS (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEBITS);

}

void
outputSWF_DEFINEBITSJPEG2 (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEBITSJPEG2);

}

void
outputSWF_DEFINEBITSJPEG3 (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEBITSJPEG3);

}

void
outputSWF_DEFINEBITSPTR (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEBITSPTR);

}

void
outputSWF_DEFINEBUTTON (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEBUTTON);

}

void
outputSWF_DEFINEBUTTON2 (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEBUTTON2);

}

void
outputSWF_DEFINEBUTTONCXFORM (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEBUTTONCXFORM);

}

void
outputSWF_DEFINEBUTTONSOUND (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEBUTTONSOUND);

}

void
outputSWF_DEFINECOMMANDOBJ (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINECOMMANDOBJ);

}

void
outputSWF_DEFINEEDITTEXT (SWF_Parserstruct * pblock)
{
  OUT_BEGIN (SWF_DEFINEEDITTEXT);

  printf (" CharacterID: %d\n", sblock->CharacterID);
  outputSWF_RECT (&(sblock->Bounds));
  printf (" Flags: ");
  printf (" HasText: %d ", sblock->HasText);
  printf (" WordWrap: %d ", sblock->WordWrap);
  printf (" Multiline: %d ", sblock->Multiline);
  printf (" Password: %d ", sblock->Password);
  printf (" ReadOnly: %d\n", sblock->ReadOnly);
  printf ("        ");
  printf (" HasTextColor: %d ", sblock->HasTextColor);
  printf (" HasMaxLength: %d ", sblock->HasMaxLength);
  printf (" HasFont: %d ", sblock->HasFont);
  printf (" AutoSize: %d ", sblock->AutoSize);
  printf (" HasLayout: %d\n", sblock->HasLayout);
  printf ("        ");
  printf (" NoSelect: %d ", sblock->NoSelect);
  printf (" Border: %d ", sblock->Border);
  printf (" HTML: %d ", sblock->HTML);
  printf (" UseOutlines: %d\n", sblock->UseOutlines);
  if (sblock->HasFont)
    {
      printf (" Font: ");
      printf (" FontID: %d ", sblock->FontID);
      printf (" FontHeight: %d\n", sblock->FontHeight);
    }
  if (sblock->HasTextColor)
    {
      outputSWF_RGBA (&sblock->TextColor,"");
    }
  if (sblock->HasLayout)
    {
      printf (" Layout:: ");
      printf (" Align: %d ", sblock->Align);
      printf (" LeftMargin: %d ", sblock->LeftMargin);
      printf (" RightMargin: %d ", sblock->RightMargin);
      printf (" Indent: %d ", sblock->Indent);
      printf (" Leading: %d\n", sblock->Leading);
    }
  printf (" VariableName: %s\n", sblock->VariableName);
  if (sblock->HasText)
    {
      printf (" InitialText: %s\n", sblock->InitialText);
    }
}

void
outputSWF_DEFINEFONT (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEFONT);

}

void
outputSWF_DEFINEFONT2 (SWF_Parserstruct * pblock)
{
  int i;
  OUT_BEGIN (SWF_DEFINEFONT2);

  printf (" FontID: %d\n", sblock->FontID);
  printf (" FontFlagsHasLayout: %d\n", sblock->FontFlagsHasLayout);
  printf (" FontFlagsShiftJis: %d\n", sblock->FontFlagsShiftJis);
  printf (" FontFlagsSmallText: %d\n", sblock->FontFlagsSmallText);
  printf (" FontFlagsFlagANSI: %d\n", sblock->FontFlagsFlagANSI);
  printf (" FontFlagsWideOffsets: %d\n", sblock->FontFlagsWideOffsets);
  printf (" FontFlagsWideCodes: %d\n", sblock->FontFlagsWideCodes);
  printf (" FontFlagsFlagsItalics: %d\n", sblock->FontFlagsFlagsItalics);
  printf (" FontFlagsFlagsBold: %d\n", sblock->FontFlagsFlagsBold);
  printf (" LanguageCode: %d\n", sblock->LanguageCode);
  printf (" FontNameLen: %d\n", sblock->FontNameLen);
  printf (" FontName: %s\n", sblock->FontName);
  printf (" NumGlyphs: %d\n", sblock->NumGlyphs);
  for (i = 0; i < sblock->NumGlyphs; i++)
    {
      if (sblock->FontFlagsWideOffsets)
	{
	  printf (" OffsetTable[%3.3d]: %lx\n", i,
		  sblock->OffsetTable.UI32[i]);
	}
      else
	{
	  printf (" OffsetTable[%3.3d]: %x\n", i,
		  sblock->OffsetTable.UI16[i]);
	}
    }
  if (sblock->FontFlagsWideOffsets)
    {
      printf (" CodeTableOffset: %lx\n", sblock->CodeTableOffset.UI32);
    }
  else
    {
      printf (" CodeTableOffset: %x\n", sblock->CodeTableOffset.UI16);
    }

}

void
outputSWF_DEFINEFONTINFO (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEFONTINFO);

}

void
outputSWF_DEFINELOSSLESS (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINELOSSLESS);

}

void
outputSWF_DEFINELOSSLESS2 (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINELOSSLESS2);

}

void
outputSWF_DEFINEMORPHSHAPE (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEMORPHSHAPE);

}

void
outputSWF_DEFINESHAPE (SWF_Parserstruct * pblock)
{
  OUT_BEGIN (SWF_DEFINESHAPE);

  printf (" ShapeID: %d\n", sblock->ShapeID);
  outputSWF_RECT (&(sblock->ShapeBounds));
  outputSWF_SHAPEWITHSTYLE (&(sblock->Shapes),1,"");
}

void
outputSWF_DEFINESHAPE2 (SWF_Parserstruct * pblock)
{
  OUT_BEGIN (SWF_DEFINESHAPE2);

  printf (" ShapeID: %d\n", sblock->ShapeID);
  outputSWF_RECT (&(sblock->ShapeBounds));
  outputSWF_SHAPEWITHSTYLE (&(sblock->Shapes),2,"");

}

void
outputSWF_DEFINESHAPE3 (SWF_Parserstruct * pblock)
{
  OUT_BEGIN (SWF_DEFINESHAPE3);

  printf (" ShapeID: %d\n", sblock->ShapeID);
  outputSWF_RECT (&(sblock->ShapeBounds));
  outputSWF_SHAPEWITHSTYLE (&(sblock->Shapes),2,"");

}

void
outputSWF_DEFINESOUND (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINESOUND);

}

void
outputSWF_DEFINESPRITE (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINESPRITE);

}

void
outputSWF_DEFINETEXT (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINETEXT);

}

void
outputSWF_DEFINETEXT2 (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINETEXT2);

}

void
outputSWF_DEFINETEXTFORMAT (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINETEXTFORMAT);

}

void
outputSWF_DEFINEVIDEO (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEVIDEO);

}

void
outputSWF_DEFINEVIDEOSTREAM (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_DEFINEVIDEOSTREAM);

}

#ifdef NODECOMPILE
void
outputSWF_ACTION (struct SWF_ACTIONRECORD *act)
{
  printf ("  Action: %s\n", actionName (act->ActionCode));
  if (act->ActionCode >= 0x80)
    {
      printf ("  Length: %d\n", act->Length);
    }
}
#endif

void
outputSWF_DOACTION (SWF_Parserstruct * pblock)
{
	OUT_BEGIN (SWF_DOACTION);
#ifdef NODECOMPILE
	int i;

	printf(" %d Actions\n", sblock->numActions);
	for(i=0;i<sblock->numActions;i++)
	outputSWF_ACTION(&sblock->Actions[i]);
#else
	printf (" %s\n", sblock->AScript);
#endif

}

void
outputSWF_ENABLEDEBUGGER (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_ENABLEDEBUGGER);

}

void
outputSWF_END (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_END);

}

void
outputSWF_EXPORTASSETS (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_EXPORTASSETS);

}

void
outputSWF_FONTREF (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_FONTREF);

}

void
outputSWF_FRAMELABEL (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_FRAMELABEL);

}

void
outputSWF_FRAMETAG (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_FRAMETAG);

}

void
outputSWF_FREEALL (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_FREEALL);

}

void
outputSWF_FREECHARACTER (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_FREECHARACTER);

}

void
outputSWF_GENCOMMAND (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_GENCOMMAND);

}

void
outputSWF_IMPORTASSETS (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_IMPORTASSETS);

}

void
outputSWF_JPEGTABLES (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_JPEGTABLES);

}

void
outputSWF_NAMECHARACTER (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_NAMECHARACTER);

}

void
outputSWF_PATHSAREPOSTSCRIPT (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_PATHSAREPOSTSCRIPT);

}

void
outputSWF_PLACEOBJECT (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_PLACEOBJECT);

}

void
outputSWF_PLACEOBJECT2 (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_PLACEOBJECT2);

}

void
outputSWF_PREBUILT (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_PREBUILT);

}

void
outputSWF_PREBUILTCLIP (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_PREBUILTCLIP);

}

void
outputSWF_PROTECT (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_PROTECT);

}

void
outputSWF_REMOVEOBJECT (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_REMOVEOBJECT);

}

void
outputSWF_REMOVEOBJECT2 (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_REMOVEOBJECT2);

}

void
outputSWF_SERIALNUMBER (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_SERIALNUMBER);

}

void
outputSWF_SETBACKGROUNDCOLOR (SWF_Parserstruct * pblock)
{
  OUT_BEGIN (SWF_SETBACKGROUNDCOLOR);

  outputSWF_RGBA (&sblock->rgb, "");

}

void
outputSWF_SHOWFRAME (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_SHOWFRAME);

}

void
outputSWF_SOUNDSTREAMBLOCK (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_SOUNDSTREAMBLOCK);

}

void
outputSWF_SOUNDSTREAMHEAD (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_SOUNDSTREAMHEAD);

}

void
outputSWF_SOUNDSTREAMHEAD2 (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_SOUNDSTREAMHEAD2);

}

void
outputSWF_STARTSOUND (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_STARTSOUND);

}

void
outputSWF_SYNCFRAME (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_SYNCFRAME);

}

void
outputSWF_VIDEOFRAME (SWF_Parserstruct * pblock)
{
  //OUT_BEGIN (SWF_VIDEOFRAME);

}

void
printRect(struct Rect *r)
{
	printf("(%i,%i)x(%i,%i)", r->xMin, r->xMax, r->yMin, r->yMax);
}

void
outputHeader (struct Movie *m)
{
	printf("File size: %i\n", m->size);

	printf("Frame size: ");
	printRect(&(m->frame));
	putchar('\n');

	printf("Frame rate: %f / sec.\n", m->rate);
	printf("Total frames: %i\n", m->nFrames);
}

void
outputTrailer ()
{
}

void
outputBlock (int type, SWF_Parserstruct * blockp, int offset, int length)
{
  int i;

  if (type < 0 || type > numOutputs)
    return;

  printf( "\nOffset: %d (%8.8x)\n", offset, offset );
  printf( "Block type: %d (%s)\n", type, blockName(type) );
  printf( "Block length: %d\n", length );

  for (i = 0; i < numOutputs; i++)
    {
      if (outputs[i].type == type)
	{
	  return outputs[i].output (blockp);
	}
    }
  return;
}